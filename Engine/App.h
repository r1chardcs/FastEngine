//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_APP_H
#define FASTENGINE_APP_H

#include "GameObject.h"
#include "Window.h"
#include "RenderSystem.h"
#include "../Toolkit/Debug/SelfCrashDumper.h"

#include "Scene.h"
#include "Toolkit/Assets/Assets.h"
#include "Toolkit/Data/LocalStorage.h"

enum class TypeEvent {
    POST, PRE
};

class App {
    SELF_PTR<Window> window;
    SELF_PTR<RenderSystem> render_system;
    SELF_PTR<THREAD> render_thread;

    STRING app_name;
    BOOL is_run;
    BOOL first_call;
    STATUS exit_code = 0;

    void Render();

    std::chrono::milliseconds logic_tick{16};

    QUEUE<FUNC<VOID(VIEW_PTR<App>)>> queue_render;
    QUEUE<FUNC<VOID(VIEW_PTR<App>)>> queue_logic;
    MUTEX mutex_render, mutex_logic;

    COMMENT("Usage: Scene")
    DEPRECTED_API
    LIST<GLOBAL_PTR<GameObject>> game_objects;
    MUTEX mutex_game_objects;

    static VIEW_PTR<App> instance;
    GLOBAL_PTR<Scene> current_scene;
    GLOBAL_PTR<Scene> scene_expectations;
    MUTEX mutex_scene;
protected:
    VIRTUAL void ProcessRenderQueue();
    VIRTUAL void ProcessLogicQueue();

    SELF_PTR<Assets> assets;
    SELF_PTR<LocalStorage> local_storage;
    void UpdateAppDebugObjectInfo();
public:
    App(MOVE_PLEASE STRING app_name);
    static App& GetInstance();

    VIEW_PTR<Scene> GetExpectationsScene() const;
    void SetExpectationsScene(const GLOBAL_PTR<Scene>& scene);

    void SetScene(const GLOBAL_PTR<Scene>& scene);
    VIEW_PTR<Scene> GetScene() const;

    void AddGameObject(const GLOBAL_PTR<GameObject> &game_object);
    void DeleteGameObject(VIEW_PTR<GameObject> game_object);
    LIST<VIEW_PTR<GameObject>> GetGameObjectByTags(const STRING& tag) const;

    template <typename TemplateComponent>
    LIST<VIEW_PTR<TemplateComponent>> GetComponents();

    VIRTUAL void Start();
    VIRTUAL void Update();
    VIRTUAL void Finish();
    VIRTUAL void World(TypeEvent type);
    VIRTUAL void UI(TypeEvent type);

    VIRTUAL void ExecuteInRenderThread(FUNC<VOID(VIEW_PTR<App>)> callback);
    VIRTUAL void ExecuteInLogicThread(FUNC<VOID(VIEW_PTR<App>)> callback);
    VIRTUAL void GrabSelfCrash(const CrashContext& ctx);

    INT GetKey(INT key) const;
    INT GetMouseKey(INT key) const;
    Vec2f GetMousePos() const;

    DOUBLE GetDeltaTime() const;

    DOUBLE GetRawFPS() const;
    DOUBLE GetFPS() const;

    STATUS Run();
    STRING GetName();

    void Exit(STATUS _exit_code);
    VIRTUAL ~App() = default;

    VIEW_PTR<Window> GetWindow() const;
    VIEW_PTR<RenderSystem> GetRenderSystem() const;
};

template<typename TemplateComponent>
LIST<VIEW_PTR<TemplateComponent>> App::GetComponents() {
    if (!current_scene) return {};
    return current_scene->GetComponents<TemplateComponent>();
}

#endif //FASTENGINE_APP_H
