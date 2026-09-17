//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_APP_H
#define FASTENGINE_APP_H

#include "Object.h"
#include "Window.h"
#include "RenderSystem.h"
#include "../Toolkit/Debug/SelfCrashDumper.h"

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

    LIST<GLOBAL_PTR<GameObject>> game_objects;
    MUTEX mutex_game_objects;

    static VIEW_PTR<App> instance;
protected:
    VIRTUAL void ProcessRenderQueue();
    VIRTUAL void ProcessLogicQueue();
public:
    App(MOVE_PLEASE STRING app_name);
    static App& GetInstance();

    void AddGameObject(const GLOBAL_PTR<GameObject> &game_object);
    void DeleteGameObject(VIEW_PTR<GameObject> game_object);
    LIST<VIEW_PTR<GameObject>> GetGameObjectByTags(const STRING& tag) const;

    template <typename TemplateComponent>
    LIST<VIEW_PTR<TemplateComponent>> GetComponents();

    VIRTUAL void Start();
    VIRTUAL void Update();
    VIRTUAL void Finish();

    VIRTUAL void ExecuteInRenderThread(FUNC<VOID(VIEW_PTR<App>)> callback);
    VIRTUAL void ExecuteInLogicThread(FUNC<VOID(VIEW_PTR<App>)> callback);
    VIRTUAL void GrabSelfCrash(const CrashContext& ctx);

    INT GetKey(INT key);
    INT GetPressKey(INT key);
    INT GetMouseKey(INT key);
    Vec2f GetMousePos();

    DOUBLE GetDeltaTime();

    STATUS Run();
    STRING GetName();

    void Exit(STATUS _exit_code);
    VIRTUAL ~App() = default;

    VIEW_PTR<Window> GetWindow() const;
    VIEW_PTR<RenderSystem> GetRenderSystem() const;
};

template<typename TemplateComponent>
LIST<VIEW_PTR<TemplateComponent>> App::GetComponents() {
    LIST<VIEW_PTR<TemplateComponent>> result;

    for (const auto& game_object : game_objects) {
        if (auto component = game_object->GetComponent<TemplateComponent>()) {
            result.push_back(component);
        }
    }

    return result;
}

#endif //FASTENGINE_APP_H
