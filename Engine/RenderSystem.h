//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_RENDERENGINE_H
#define FASTENGINE_RENDERENGINE_H

#include <Platform.h>

#include "Render2D.h"
#include "../Toolkit/Quat.h"
#include "../Toolkit/Vector.h"

class App; /*include "Engine/app.h";*/

class Camera {
    FLOAT x, y, height, scale;
public:
    Camera();
    Camera(FLOAT height, FLOAT scale);

    FLOAT GetX() const;
    FLOAT GetY() const;
    FLOAT GetHeight() const;
    FLOAT GetScale() const;

    void SetX(FLOAT value);
    void SetY(FLOAT value);
    void SetHeight(FLOAT value);
    void SetScale(FLOAT value);
};

class RenderSystem {
    SELF_PTR<Camera> camera = nullptr;
    VIEW_PTR<App> app = nullptr;
    DOUBLE lastTime = 0, deltaTime = 0;
    void UpdateDeltaTime();

    CALLBACK<VOID(VIEW_PTR<RenderSystem>)> ui_render_callback;
    CALLBACK<VOID(VIEW_PTR<RenderSystem>)> world_render_callback;

    HASH_MAP<STRING, GLOBAL_PTR<Texture>> textures;
    mutable MUTEX mutex_textures;
    INT lastWidth = 1;
    INT lastHeight = 1;
    DOUBLE fps = 0.0;
    DOUBLE fps_accum_time = 0.0;
    INT fps_frame_count = 0;

    RGBA backgroundColor = {.r = 0, .g = 0, .b = 0, .a = 1};
public:
    RenderSystem(VIEW_PTR<App> app);

    RGBA& BackgroundColor();

    void SetRenderUICallback(const CALLBACK<VOID(VIEW_PTR<RenderSystem>)> &callback);
    void SetRenderWorldCallback(const CALLBACK<VOID(VIEW_PTR<RenderSystem>)> &callback);

    void OnResize(INT width, INT height);
    void OnUpdate();

    static void Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z);

    static void Rotate(const Quat& quat);

    static void NewContext();
    BOOL IsInView(Vec2f pos, Vec2f size) const;
    static void StopContext();

    void StartWorld() const;
    void EndWorld() const;

    void StartUI() const;
    void EndUI() const;

    void LoadTexture(LITERAL path);

    GLOBAL_PTR<Texture> LoadTextureSync(LITERAL path);

    GLOBAL_PTR<Texture> GetTexture(LITERAL path) const;

    GLOBAL_PTR<Texture> GetTexture(LITERAL path);

    VIEW_PTR<Camera> GetCamera() const;
    VIEW_PTR<App> GetApp() const;

    DOUBLE GetDeltaTime() const;

    Vec2f GetScreenSize() const;
    Vec2f CenterPositionScreen() const;

    DOUBLE GetFPS() const;
};

#endif //FASTENGINE_RENDERENGINE_H
