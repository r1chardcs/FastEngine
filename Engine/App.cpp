//
// Created by dlllibstdntc on 12.09.2026.
//

#include "App.h"

#include <sstream>

#include "GameObject.h"
#include "../Toolkit/Debug/Logger.h"
#include "../Toolkit/Debug/Test.h"
#include "../Toolkit/Debug/LagProfiler.h"
#include "../Toolkit/IO/IO.h"
#include "../Toolkit/Input.h"

void App::Render() {
    window = MakeSelfPtr<Window>(this->app_name.c_str(), 800, 800);
    window->SetResizeCallback([this](auto, auto w, auto h) {
        if (render_system) render_system->OnResize(w, h);
    });
    window->MakeContext();

    while (window->IsRun() && is_run) {
        toolkit::profiler::lag::Push("Engine::App::Render");
        ProcessRenderQueue();
        if (render_system) {
            render_system->OnUpdate();
        }
        window->SwapBuffer();
        toolkit::profiler::lag::Pop();
    }

    if (is_run) {
        is_run = false;
    }
}

VIEW_PTR<App> App::instance = nullptr;


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

    toolkit::scd::SetCallback([this](auto ctx) { this->GrabSelfCrash(ctx); });
    toolkit::scd::AttachHandler();

    assets = MakeSelfPtr<Assets>("Assets");
    local_storage = MakeSelfPtr<toolkit::LocalStorage>("data", "global.dat");

    if (!local_storage->Load()) {
        if (const auto [res, err] = toolkit::io::file::ExistDirectory("data"); !res || err) {
            if (err) {
                toolkit::LOGERR.Output("Filesystem error: %s\n", err);
            }
            else {
                toolkit::io::file::CreateDirectory("data");
            }
        }
        else {
            toolkit::LOGWRN.Output("Error load localstorage: %s\n", local_storage->GetFullPath().c_str());
        }
    }
}

App & App::GetInstance() {
    return *instance;
}

VIEW_PTR<Scene> App::GetExpectationsScene() const {
    return scene_expectations.get();
}

void App::SetExpectationsScene(const GLOBAL_PTR<Scene> &scene) {
    this->scene_expectations = scene;
}

void App::SetScene(const GLOBAL_PTR<Scene>& scene) {
    if (!scene) {
        toolkit::LOGWRN.Output("SetScene called with null scene");
        return;
    }

    if (!scene_expectations) {
        MUTEX_LOCK lock(mutex_scene);
        if (current_scene) {
            current_scene->Finish();
        }
        current_scene = scene;
        current_scene->Setup();
        current_scene->Start();
        return;
    }

    {
        MUTEX_LOCK lock(mutex_scene);
        if (current_scene) {
            current_scene->Finish();
        }
        current_scene = scene_expectations;
    }
    scene_expectations->Start();
    scene_expectations->Setup();

    ExecuteInLogicThread([this, scene](VIEW_PTR<App>) {
        temp_current_scene = scene;

        scene->Start();
        scene->Setup();

        MUTEX_LOCK lock(mutex_scene);
        if (current_scene.get() == scene_expectations.get()) {
            scene_expectations->Finish();
        }
        current_scene = scene;
        temp_current_scene = nullptr;
    });
}

VIEW_PTR<Scene> App::GetScene() const {
    if (temp_current_scene) {
        return temp_current_scene.get();
    }
    return current_scene.get();
}
void App::AddGameObject(const GLOBAL_PTR<GameObject>& game_object) {
    MUTEX_LOCK lock(mutex_scene);
    if (!current_scene) {
        toolkit::LOGWRN.Output("AddGameObject called with no active scene");
        return;
    }
    current_scene->AddGameObject(game_object);
}

void App::DeleteGameObject(const VIEW_PTR<GameObject> game_object) {
    MUTEX_LOCK lock(mutex_scene);
    if (!current_scene) {
        toolkit::LOGERR.Output("The scene is not set.");
        return;
    }
    current_scene->DeleteGameObject(game_object);
}

LIST<VIEW_PTR<GameObject>> App::GetGameObjectByTags(const STRING &tag) const {
    if (!current_scene) {
        toolkit::LOGERR.Output("The scene is not set.");
        return {};
    }
    return current_scene->GetGameObjectByTags(tag);
}

void App::Start() {

}

void App::Update() {
}

void App::Finish() {
}

void App::World(TypeEvent type) {
}

void App::UI(TypeEvent type) {
}

void App::ExecuteInRenderThread(const FUNC<VOID(VIEW_PTR<App>)> callback) {
    MUTEX_LOCK lock(mutex_render);
    queue_render.push(callback);
}

void App::ExecuteInLogicThread(const FUNC<void(VIEW_PTR<App>)> callback) {
    MUTEX_LOCK lock(mutex_logic);
    queue_logic.push(callback);
}

void App::GrabSelfCrash(const toolkit::CrashContext &ctx) {
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
    toolkit::io::file::WriteFile(filename, content);
}

INT App::GetKey(const INT key) const {
    return window->GetKey(key);
}

INT App::GetMouseKey(const INT key) const {
    return window->GetMouseKey(key);
}

toolkit::Vec2f App::GetMousePos() const {
    return window->GetMousePosition();
}

DOUBLE App::GetDeltaTime() const {
    return render_system->GetDeltaTime();
}

DOUBLE App::GetRawFPS() const {
    const DOUBLE dt = GetDeltaTime();
    return dt > 0.0 ? 1.0 / dt : 0.0;
}

DOUBLE App::GetFPS() const {
    return render_system->GetFPS();
}

STATUS App::Run() {
    is_run = true;
    first_call = false;

    render_system = MakeSelfPtr<RenderSystem>(this);
    render_thread = MakeSelfPtr<THREAD>([this]() { this->Render(); });
    render_thread->detach();

    toolkit::input::InitializeInput();

    auto next_tick = std::chrono::steady_clock::now();

    render_system->SetRenderWorldCallback([this](auto) {
        toolkit::profiler::lag::Push("RenderSystem::World::Render");
        World(TypeEvent::PRE);

        if (current_scene) current_scene->Render();

        World(TypeEvent::POST);
        toolkit::profiler::lag::Pop();
    });

    render_system->SetRenderUICallback([this](auto) {
        toolkit::profiler::lag::Push("RenderSystem::UI::Render");
        UI(TypeEvent::PRE);
        if (current_scene)
            current_scene->UI();
        UI(TypeEvent::POST);
        toolkit::profiler::lag::Pop();
    });

    while (is_run) {
        toolkit::profiler::lag::Push("Engine::App::Logic");

        if (first_call == false) {
            if (window) {
                Start();
                first_call = true;
            }
        }
        if (window) {
            toolkit::input::UpdateKeys();
            Update();
        }

        if (current_scene) current_scene->Update();

        ProcessLogicQueue();

        next_tick += logic_tick;
        std::this_thread::sleep_until(next_tick);
        toolkit::profiler::lag::Pop();
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
