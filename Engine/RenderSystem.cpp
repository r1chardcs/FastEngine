//
// Created by dlllibstdntc on 12.09.2026.
//

#include "RenderSystem.h"

#include <future>

#include "App.h"

#include "../Toolkit/Debug/Logger.h"

#include <GL/gl.h>


Camera::Camera(): x(0), y(0), height(10), scale(1) {}

Camera::Camera(const FLOAT height, const FLOAT scale): x(0), y(0), height(height), scale(scale) {}

FLOAT Camera::GetX() const {
    return x;
}

FLOAT Camera::GetY() const {
    return y;
}

FLOAT Camera::GetHeight() const {
    return height;
}

FLOAT Camera::GetScale() const {
    return scale;
}

void Camera::SetX(const FLOAT value) {
    x = value;
}

void Camera::SetY(const FLOAT value) {
    y = value;
}

void Camera::SetHeight(const FLOAT value) {
    height = value;
}

void Camera::SetScale(const FLOAT value) {
    scale = value;
}

void RenderSystem::UpdateDeltaTime() {
    const auto now = app->GetWindow()->GetTime();
    deltaTime = now - lastTime;
    lastTime = now;

    fps_accum_time += GetDeltaTime();
    fps_frame_count++;

    if (fps_accum_time >= 1.0) {
        fps = fps_frame_count / fps_accum_time;
        fps_accum_time = 0.0;
        fps_frame_count = 0;
    }
}

RenderSystem::RenderSystem(VIEW_PTR<App> app): app(app) {
    camera = MakeSelfPtr<Camera>();
}

RGBA & RenderSystem::BackgroundColor() {
    return backgroundColor;
}

void RenderSystem::UpdateCamera() {
}

void RenderSystem::SetRenderUICallback(const FUNC<void(VIEW_PTR<RenderSystem>)> &callback) {
    ui_render_callback = callback;
}

void RenderSystem::SetRenderWorldCallback(const FUNC<void(VIEW_PTR<RenderSystem>)> &callback) {
    world_render_callback = callback;
}

void RenderSystem::OnResize(INT width, INT height) {
    if (height <= 0) height = 1;

    glViewport(0, 0, width, height);

    lastWidth = width;
    lastHeight = height;
}

void RenderSystem::OnUpdate() {
    UpdateDeltaTime();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

    StartWorld();
    {
        if (world_render_callback) world_render_callback(this);
    }
    EndWorld();

    StartUI();
    {
        if (ui_render_callback) ui_render_callback(this);
    }
    EndUI();
}

void RenderSystem::Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z) {
    glRotatef(angle, x, y, z);
}

void RenderSystem::Rotate(const Quat& quat) {
    FLOAT matrix[16];
    quat.ToMatrix4x4(matrix);
    glMultMatrixf(matrix);
}

void RenderSystem::NewContext() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

BOOL RenderSystem::IsInView(Vec2f pos, Vec2f size) const {
    const FLOAT aspect = static_cast<FLOAT>(lastWidth) / static_cast<FLOAT>(lastHeight);

    const FLOAT camHeight = camera->GetHeight() / camera->GetScale();
    const FLOAT camWidth = camHeight * aspect;

    const FLOAT viewLeft   = camera->GetX() - camWidth;
    const FLOAT viewRight  = camera->GetX() + camWidth;
    const FLOAT viewBottom = camera->GetY() - camHeight;
    const FLOAT viewTop    = camera->GetY() + camHeight;

    const FLOAT halfW = size.x / 2.0f;
    const FLOAT halfH = size.y / 2.0f;

    const FLOAT objLeft   = pos.x - halfW;
    const FLOAT objRight  = pos.x + halfW;
    const FLOAT objBottom = pos.y - halfH;
    const FLOAT objTop    = pos.y + halfH;

    return objRight  >= viewLeft
        && objLeft   <= viewRight
        && objTop    >= viewBottom
        && objBottom <= viewTop;
}

void RenderSystem::StopContext() {
    glPopMatrix();
}

void RenderSystem::StartWorld() const {
    const FLOAT aspect = static_cast<FLOAT>(lastWidth) / static_cast<FLOAT>(lastHeight);

    const FLOAT camHeight = camera->GetHeight() / camera->GetScale();
    const FLOAT camWidth = camHeight * aspect;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-camWidth, camWidth, -camHeight, camHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-camera->GetX(), -camera->GetY(), 0);
}

void RenderSystem::EndWorld() const {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void RenderSystem::StartUI() const {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, app->GetWindow()->GetWidth(),
            app->GetWindow()->GetHeight(), 0,
            -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void RenderSystem::EndUI() const {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void RenderSystem::LoadTexture(const LITERAL path) {
    STRING key(path);
    app->ExecuteInRenderThread([this, key](auto) {
        auto [res, err] = Render2D::GetTexture(key.c_str());
        if (err) {
            LOGWRN.Output("%s", err);
        }

        MUTEX_LOCK lock(mutex_textures);
        this->textures[key] = res;
    });
}

GLOBAL_PTR<Texture> RenderSystem::LoadTextureSync(const LITERAL path) {
    STRING key(path);
    auto promise = std::make_shared<std::promise<GLOBAL_PTR<Texture>>>();
    std::future<GLOBAL_PTR<Texture>> future = promise->get_future();

    app->ExecuteInRenderThread([this, key, promise](auto) {
        auto [res, err] = Render2D::GetTexture(key.c_str());
        if (err) {
            LOGWRN.Output("%s", err);
        }

        {
            MUTEX_LOCK lock(mutex_textures);
            this->textures[key] = res;
        }

        promise->set_value(res);
    });

    return future.get();
}

GLOBAL_PTR<Texture> RenderSystem::GetTexture(const LITERAL path) const {
    MUTEX_LOCK lock(mutex_textures);
    const auto it = textures.find(path);
    if (it == textures.end()) {
        LOGWRN.Output("Texture not loaded: %s", path);
        return nullptr;
    }
    return it->second;
}

GLOBAL_PTR<Texture> RenderSystem::GetTexture(const LITERAL path) {
    MUTEX_LOCK lock(mutex_textures);
    const auto it = textures.find(path);
    if (it == textures.end()) {
        LOGWRN.Output("Texture not loaded: %s", path);
        return nullptr;
    }
    return it->second;
}

VIEW_PTR<Camera> RenderSystem::GetCamera() const {
    return camera.get();
}

VIEW_PTR<App> RenderSystem::GetApp() const {
    return app;
}

DOUBLE RenderSystem::GetDeltaTime() const {
    return deltaTime;
}

Vec2f RenderSystem::GetScreenSize() const {
    return {
            static_cast<FLOAT>(app->GetWindow()->GetWidth()),
            static_cast<FLOAT>(app->GetWindow()->GetHeight())};
}

Vec2f RenderSystem::CenterPositionScreen() const {
    const auto size = GetScreenSize();
    return {
        size.x / 2, size.y / 2
    };
}

DOUBLE RenderSystem::GetFPS() const {
    return fps;
}