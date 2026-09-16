//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_RENDERENGINE_H
#define FASTENGINE_RENDERENGINE_H

#include <Platform.h>

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
public:
    RenderSystem(VIEW_PTR<App> app);

    void SetRenderUICallback(const CALLBACK<VOID(VIEW_PTR<RenderSystem>)> &callback);
    void SetRenderWorldCallback(const CALLBACK<VOID(VIEW_PTR<RenderSystem>)> &callback);

    void OnResize(INT width, INT height);
    void OnUpdate();

    void StartWorld();
    void EndWorld();

    void StartUI();
    void EndUI();

    VIEW_PTR<Camera> GetCamera() const;
    VIEW_PTR<App> GetApp() const;

    DOUBLE GetDeltaTime() const;

    Vec2f GetScreenSize() const;
    Vec2f CenterPositionScreen() const;
};

#endif //FASTENGINE_RENDERENGINE_H
