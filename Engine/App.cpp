//
// Created by dlllibstdntc on 12.09.2026.
//

#include "App.h"

#include "../Toolkit/Debug/Test.h"

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
    is_run = false;
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

STATUS App::Run() {
    is_run = true;
    first_call = false;

    render_system = MakeSelfPtr<RenderSystem>(this);
    render_thread = MakeSelfPtr<THREAD>([this]() { this->Render(); });
    render_thread->detach();

    auto next_tick = std::chrono::steady_clock::now();

    while (is_run) {
        Update();
        ProcessLogicQueue();

        next_tick += logic_tick;
        std::this_thread::sleep_until(next_tick);

        if (first_call == false) {
            Start();
            first_call = true;
        }
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
