//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_SCENE_H
#define FASTENGINE_SCENE_H

#include <Engine/GameObject.h>

class Scene {
    LIST<GLOBAL_PTR<GameObject>> game_objects;
    mutable MUTEX mutex_objects;

    STRING scene_name;
    BOOL is_started = false;

public:
    explicit Scene(MOVE_PLEASE STRING scene_name);
    VIRTUAL ~Scene() = default;

    VIRTUAL void Start();
    VIRTUAL void Update();
    VIRTUAL void Render();
    VIRTUAL void Finish();

    VIRTUAL void UI();

    void AddGameObject(const GLOBAL_PTR<GameObject>& game_object);
    void DeleteGameObject(VIEW_PTR<GameObject> game_object);

    LIST<VIEW_PTR<GameObject>> GetGameObjectByTags(const STRING& tag) const;

    template <typename TemplateComponent>
    LIST<VIEW_PTR<TemplateComponent>> GetComponents();

    LIST<GLOBAL_PTR<GameObject>> Snapshot() const;

    STRING GetName() const;
    BOOL IsStarted() const;

    DOUBLE GetDeltaTime();

    INT GetGameObjectSize() const;
};

template<typename TemplateComponent>
LIST<VIEW_PTR<TemplateComponent>> Scene::GetComponents() {
    LIST<VIEW_PTR<TemplateComponent>> result;

    const auto objects = Snapshot();
    for (const auto& game_object : objects) {
        if (auto component = game_object->GetComponent<TemplateComponent>()) {
            result.push_back(component);
        }
    }

    return result;
}

#endif //FASTENGINE_SCENE_H
