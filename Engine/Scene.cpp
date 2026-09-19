//
// Created by dlllibstdntc on 17.09.2026.
//

#include "Scene.h"

#include "App.h"
#include "GameObject.h"
#include "../Toolkit/Debug/Logger.h"
#include "Components/Transform.h"
#include "Toolkit/Anim/AnimationSystem.h"
#include "Toolkit/Debug/Test.h"

Scene::Scene(STRING scene_name) : scene_name(MOVE(scene_name)) {}

void Scene::Setup() {
    GetRenderSystem()->BackgroundColor() = GetBackgroundColor();
}

void Scene::Start() {
    is_started = true;
}

void Scene::Update() {
    AnimationSystem::GetInstance()
        .Update(static_cast<FLOAT>(App::GetInstance().GetDeltaTime()));

    const auto snapshot = Snapshot();
    for (const auto& game_object : *snapshot) {
        TEST(!game_object && "Invalid Game Object at Snapshot");
        if (!game_object->IsActive()) continue;

        game_object->Update();
        for (const auto& component : game_object->GetComponents()) {
            if (component) component->Update();
            else { TEST(true && "Invalid component at object") }
        }
    }
}

void Scene::Render() {
    const auto snapshot = Snapshot();
    const auto render_system = App::GetInstance().GetRenderSystem();

    for (const auto& game_object : *snapshot) {
        if (!game_object->IsActive()) continue;
        if (const auto transform = game_object->GetComponent<Transform>()) {
            if (!render_system->IsInView(transform->HalfPos().ToVec2(), transform->Size().ToVec2())) continue;
        }

        render_system->NewContext();

        const auto& components = game_object->GetComponents();

        for (const auto& component : components) {
            if (component) {
                component->Render();
                component->Render(Component::Type::Pre);
            } else { TEST(true && "Invalid component at object") }
        }

        game_object->DrawWorld();

        for (const auto& component : components) {
            if (component) {
                component->Render(Component::Type::Post);
                component->Render();
            }
        }

        render_system->StopContext();
    }
}

void Scene::Finish() {
    GLOBAL_PTR<const ObjectList> objects;
    {
        MUTEX_LOCK lock(mutex_objects);
        objects = game_objects;
        game_objects = MakeGlobalPtr<const ObjectList>();
    }

    for (const auto& game_object : *objects) {
        if (!game_object) continue;
        for (const auto& component : game_object->GetComponents()) {
            if (component) component->Shutdown();
        }
        game_object->Shutdown();
    }

    is_started = false;
}

RGBA Scene::GetBackgroundColor() {
    return {.r = 0, .g = 0, .b = 0, .a = 1};
}

void Scene::UI() {
    const auto snapshot = Snapshot();
    const auto render_system = App::GetInstance().GetRenderSystem();

    for (const auto& game_object : *snapshot) {
        if (!game_object->IsActive()) continue;

        render_system->NewContext();
        game_object->DrawUI();
        render_system->StopContext();
    }
}

void Scene::AddGameObject(const GLOBAL_PTR<GameObject>& game_object) {
    if (!game_object || !game_object->IsActive()) {
        LOGWRN.Output("Add Invalid game object at 0x%p", game_object.get());
        return;
    }
    game_object->Start();

    MUTEX_LOCK lock(mutex_objects);
    auto new_list = MakeGlobalPtr<ObjectList>(*game_objects);
    new_list->push_back(game_object);
    game_objects = MOVE(new_list);
}

void Scene::DeleteGameObject(VIEW_PTR<GameObject> game_object) {
    if (!game_object) {
        LOGWRN.Output("Delete Invalid game object at 0x%p", game_object);
        return;
    }

    for (const auto& component : game_object->GetComponents()) {
        if (component) component->Shutdown();
    }
    game_object->Shutdown();

    MUTEX_LOCK lock(mutex_objects);
    auto new_list = MakeGlobalPtr<ObjectList>();
    new_list->reserve(game_objects->size());
    for (const auto& obj : *game_objects) {
        if (obj.get() != static_cast<GameObject*>(game_object)) {
            new_list->push_back(obj);
        }
    }
    game_objects = MOVE(new_list);
}

VIEW_PTR<RenderSystem> Scene::GetRenderSystem() {
    return GetApp().GetRenderSystem();
}

App& Scene::GetApp() {
    return App::GetInstance();
}

LIST<VIEW_PTR<GameObject>> Scene::GetGameObjectByTags(const STRING &tag) const {
    LIST<VIEW_PTR<GameObject>> tags;

    const auto snapshot = Snapshot();
    for (const auto& game_object : *snapshot) {
        if (!game_object) {
            TEST(true && "Invalid Game Object at Snaphsot")
            continue;
        }
        for (const auto& obj_tag : game_object->GetTags()) {
            if (obj_tag == tag) {
                tags.push_back(game_object.get());
                break;
            }
        }
    }

    return tags;
}

GLOBAL_PTR<const Scene::ObjectList> Scene::Snapshot() const {
    MUTEX_LOCK lock(mutex_objects);
    return game_objects;
}

STRING Scene::GetName() const {
    return scene_name;
}

BOOL Scene::IsStarted() const {
    return is_started;
}

DOUBLE Scene::GetDeltaTime() {
    return App::GetInstance().GetDeltaTime();
}

INT Scene::GetGameObjectSize() const {
    return static_cast<INT>(game_objects->size());
}
