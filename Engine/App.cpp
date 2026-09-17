//
// Created by dlllibstdntc on 12.09.2026.
//

#include "App.h"

#include <sstream>

#include "glfw3.h"
#include "Object.h"
#include "../Toolkit/Debug/Logger.h"
#include "../Toolkit/Debug/Test.h"
#include "../Toolkit/IO/IO.h"

VIEW_PTR<App> App::instance = nullptr;

void App::Render() {
    window = MakeSelfPtr<Window>(this->app_name.c_str(), 800, 800);
    window->SetResizeCallback([this](auto sender, auto w, auto h) {
        if (render_system) render_system->OnResize(w, h);
    });
    window->MakeContext();

    while (window->IsRun() && is_run) {
        ProcessRenderQueue();
        if (render_system) {
            render_system->OnUpdate();
        }
        window->SwapBuffer();
    }

    if (is_run) {
        is_run = false;
    }
}

void App::ProcessRenderQueue() {
    QUEUE<FUNC<VOID(VIEW_PTR<App>)>> tasks;
    {
        MUTEX_LOCK lock(mutex_render);
        std::swap(tasks, queue_render);
    }
    while (!tasks.empty()) {
        tasks.front().operator()(this);
        tasks.pop();
    }
}

void App::ProcessLogicQueue() {
    QUEUE<FUNC<VOID(VIEW_PTR<App>)>> tasks;
    {
        MUTEX_LOCK lock(mutex_logic);
        std::swap(tasks, queue_logic);
    }
    while (!tasks.empty()) {
        tasks.front().operator()(this);
        tasks.pop();
    }
}

App::App(STRING app_name) : app_name(MOVE(app_name)) {
    instance = this;
    is_run = false;

    CrashDumper::SetCallback([this](auto ctx) { this->GrabSelfCrash(ctx); });
    CrashDumper::AttachHandler();
}

App & App::GetInstance() {
    return *instance;
}

void App::AddGameObject(const GLOBAL_PTR<GameObject> &game_object) {
    if (!game_object || !game_object->IsActive()) {
        LOGWRN.Output("Add Invalid game object at 0x%p", game_object.get());
        return;
    }
    game_object->Start();

    MUTEX_LOCK lock(mutex_game_objects);
    this->game_objects.push_back(game_object);
}

void App::DeleteGameObject(VIEW_PTR<GameObject> game_object) {
    if (!game_object || !game_object->IsActive()) {
        LOGWRN.Output("Delete Invalid game object at 0x%p", game_object.operator->());
        return;
    }

    for (const auto components = game_object->GetComponents();
            auto component : components) if (component) component->Shutdown();

    game_object->Shutdown();

    MUTEX_LOCK lock(mutex_game_objects);
    this->game_objects.remove_if([game_object](const GLOBAL_PTR<GameObject>& obj) {
       return obj.get() == static_cast<GameObject*>(game_object);
   });
}

LIST<VIEW_PTR<GameObject>> App::GetGameObjectByTags(const STRING &tag) const {
    LIST<VIEW_PTR<GameObject>> tags;
    for (const auto &game_object : game_objects) {
        if (game_object)
            for (const auto &obj_tag : game_object->GetTags()) {
                if (obj_tag == tag) { tags.push_back(game_object.get()); }
            }
    }
    return tags;
}

void App::Start() {

}

void App::Update() {
}

void App::Finish() {
}

void App::ExecuteInRenderThread(const FUNC<VOID(VIEW_PTR<App>)> callback) {
    MUTEX_LOCK lock(mutex_render);
    queue_render.push(callback);
}

void App::ExecuteInLogicThread(FUNC<void(VIEW_PTR<App>)> callback) {
    MUTEX_LOCK lock(mutex_logic);
    queue_logic.push(callback);
}

void App::GrabSelfCrash(const CrashContext &ctx) {
    std::ostringstream ss;
    ss << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(ctx.exception_address);
    const STRING addressHex = ss.str();

    STRING content;
    content += "Exception: " + ctx.exception_description + "\n";
    content += "Address: 0x" + addressHex + "\n";
    content += "Stack trace:\n";

    for (const auto &frame : ctx.stack_trace) {
        std::ostringstream frameStream;
        frameStream << "  at " << frame.function_name;

        if (!frame.module_name.empty()) {
            frameStream << " [" << frame.module_name << "]";
        }

        if (!frame.file_name.empty()) {
            frameStream << " (" << frame.file_name << ":" << frame.line_number << ")";
        }

        content += frameStream.str() + "\n";
    }

    const STRING filename = "dump_at_" + addressHex + ".log";
    IO::File::WriteFile(filename, content);
}

INT App::GetKey(INT key) {
    return window->GetKey(key);
}

INT App::GetPressKey(INT key) {
    return window->GetPressKey(key);
}

INT App::GetMouseKey(INT key) {
    return window->GetMouseKey(key);
}

Vec2f App::GetMousePos() {
    return window->GetMousePosition();
}

DOUBLE App::GetDeltaTime() {
    return render_system->GetDeltaTime();
}

STATUS App::Run() {
    is_run = true;
    first_call = false;

    render_system = MakeSelfPtr<RenderSystem>(this);
    render_thread = MakeSelfPtr<THREAD>([this]() { this->Render(); });
    render_thread->detach();

    auto next_tick = std::chrono::steady_clock::now();

    render_system->SetRenderWorldCallback([this](auto) {
        for (const auto &game_object : game_objects) {
            if (game_object->IsActive()) {
                game_object->DrawWorld();
                for (const auto components = game_object->GetComponents();
                    auto component : components) if (component) component->Render();
            }
        }
    });

    while (is_run) {
        if (first_call == false) {
            if (window) {
                Start();
                first_call = true;
            }
        }
        if (window) {
            Update();
        }
        for (const auto &game_object : game_objects) {
            game_object->Update();
            for (const auto components = game_object->GetComponents();
                auto component : components) if (component) component->Update();
        }

        ProcessLogicQueue();

        next_tick += logic_tick;
        std::this_thread::sleep_until(next_tick);
    }
    Finish();
    render_thread.release();

    return exit_code;
}

STRING App::GetName() {
    return app_name;
}

void App::Exit(const STATUS _exit_code) {
    this->exit_code = _exit_code;
    is_run = false;
}

VIEW_PTR<Window> App::GetWindow() const {
    return window.get();
}

VIEW_PTR<RenderSystem> App::GetRenderSystem() const {
    return render_system.get();
}
