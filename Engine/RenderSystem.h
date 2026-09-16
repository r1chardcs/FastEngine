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

    HASH_MAP<STRING, Texture> textures;
    mutable MUTEX mutex_textures;
public:
    RenderSystem(VIEW_PTR<App> app);

    void SetRenderUICallback(const CALLBACK<VOID(VIEW_PTR<RenderSystem>)> &callback);
    void SetRenderWorldCallback(const CALLBACK<VOID(VIEW_PTR<RenderSystem>)> &callback);

    void OnResize(INT width, INT height);
    void OnUpdate();

    void Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z);
    void Rotate(const Quat& quat);

    void NewContext();
    void StopContext();

    void StartWorld();
    void EndWorld();

    void StartUI();
    void EndUI();

    void LoadTexture(LITERAL path);

    Texture LoadTextureSync(LITERAL path);

    Texture GetTexture(LITERAL path) const;

    Texture GetTexture(LITERAL path);

    VIEW_PTR<Camera> GetCamera() const;
    VIEW_PTR<App> GetApp() const;

    DOUBLE GetDeltaTime() const;

    Vec2f GetScreenSize() const;
    Vec2f CenterPositionScreen() const;
};

#endif //FASTENGINE_RENDERENGINE_H
