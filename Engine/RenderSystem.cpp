//
// Created by dlllibstdntc on 12.09.2026.
//

#include "RenderSystem.h"
#include "App.h"

#include "../Toolkit/Debug/Logger.h"

#include <GL/gl.h>


Camera::Camera(): x(0), y(0), height(10), scale(1) {}

Camera::Camera(FLOAT height, FLOAT scale): x(0), y(0), height(height), scale(scale) {}

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

void Camera::SetX(FLOAT value) {
    x = value;
}

void Camera::SetY(FLOAT value) {
    y = value;
}

void Camera::SetHeight(FLOAT value) {
    height = value;
}

void Camera::SetScale(FLOAT value) {
    scale = value;
}

void RenderSystem::UpdateDeltaTime() {
    const auto now = app->GetWindow()->GetTime();
    deltaTime = now - lastTime;
    lastTime = now;
}

RenderSystem::RenderSystem(VIEW_PTR<App> app): app(app) {}

void RenderSystem::SetRenderUICallback(const FUNC<void(VIEW_PTR<RenderSystem>)> &callback) {
    ui_render_callback = callback;
}

void RenderSystem::SetRenderWorldCallback(const FUNC<void(VIEW_PTR<RenderSystem>)> &callback) {
    world_render_callback = callback;
}

void RenderSystem::OnResize(INT width, INT height) {
    glViewport(0, 0, width, height);
    const FLOAT factor = static_cast<float>(width) / height;
    glLoadIdentity();
    glOrtho(-factor, factor, -1, 1, -1, 1);
}

void RenderSystem::OnUpdate() {
    UpdateDeltaTime();

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

void RenderSystem::StartWorld() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void RenderSystem::EndWorld() {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void RenderSystem::StartUI() {
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

void RenderSystem::EndUI() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
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
