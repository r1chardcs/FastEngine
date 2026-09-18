//
// Created by dlllibstdntc on 17.09.2026.
//

#include "Scene.h"

#include "App.h"
#include "GameObject.h"
#include "../Toolkit/Debug/Logger.h"
#include "Toolkit/Anim/AnimationSystem.h"

Scene::Scene(STRING scene_name) : scene_name(MOVE(scene_name)) {
}

void Scene::Start() {
    is_started = true;
}

void Scene::Update() {
    AnimationSystem::GetInstance().Update(static_cast<FLOAT>(App::GetInstance().GetDeltaTime()));

    for (const auto& game_object : Snapshot()) {
        if (!game_object->IsActive()) continue;

        game_object->Update();
        for (const auto components = game_object->GetComponents();
                auto component : components) if (component) component->Update();
    }
}

void Scene::Render() {
    for (const auto& game_object : Snapshot()) {
        if (!game_object->IsActive()) continue;

        App::GetInstance().GetRenderSystem()->NewContext();
        for (const auto components = game_object->GetComponents();
                auto component : components) if (component) {
                    component->Render();
                    component->Render(Component::Type::Pre);
                }

        game_object->DrawWorld();
        for (const auto components = game_object->GetComponents();
                auto component : components) if (component) {
                    component->Render(Component::Type::Post);
                }
        App::GetInstance().GetRenderSystem()->StopContext();
    }
}

void Scene::Finish() {
    MUTEX_LOCK lock(mutex_objects);

    for (const auto& game_object : game_objects) {
        if (!game_object) continue;
        for (const auto components = game_object->GetComponents();
                auto component : components) if (component) component->Shutdown();
        game_object->Shutdown();
    }

    game_objects.clear();
    is_started = false;
}

void Scene::UI() {
    for (const auto& game_object : Snapshot()) {
        if (!game_object->IsActive()) continue;

        App::GetInstance().GetRenderSystem()->NewContext();
        game_object->DrawUI();
        App::GetInstance().GetRenderSystem()->StopContext();
    }
}

void Scene::AddGameObject(const GLOBAL_PTR<GameObject>& game_object) {
    if (!game_object || !game_object->IsActive()) {
        LOGWRN.Output("Add Invalid game object at 0x%p", game_object.get());
        return;
    }
    game_object->Start();

    MUTEX_LOCK lock(mutex_objects);
    game_objects.push_back(game_object);
}

void Scene::DeleteGameObject(VIEW_PTR<GameObject> game_object) {
    if (!game_object) {
        LOGWRN.Output("Delete Invalid game object at 0x%p", game_object);
        return;
    }

    for (const auto components = game_object->GetComponents();
            auto component : components) if (component) component->Shutdown();
    game_object->Shutdown();

    MUTEX_LOCK lock(mutex_objects);
    game_objects.remove_if([game_object](const GLOBAL_PTR<GameObject>& obj) {
        return obj.get() == static_cast<GameObject*>(game_object);
    });
}

LIST<VIEW_PTR<GameObject>> Scene::GetGameObjectByTags(const STRING &tag) const {
    LIST<VIEW_PTR<GameObject>> tags;

    for (const auto& game_object : Snapshot()) {
        if (!game_object) continue;
        for (const auto& obj_tag : game_object->GetTags()) {
            if (obj_tag == tag) { tags.push_back(game_object.get()); }
        }
    }

    return tags;
}

LIST<GLOBAL_PTR<GameObject>> Scene::Snapshot() const {
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
    return game_objects.size();
}
