//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_SCENE_H
#define FASTENGINE_SCENE_H

#include <Engine/GameObject.h>

class Scene {
public:
    using ObjectList = VECTOR<GLOBAL_PTR<GameObject>>;
    struct DebugInfo {
        INT CountAllObject;
        INT CountRenderObject;
        INT CountNotRenderObject;
        INT CountDisableObject;
    };
private:
    GLOBAL_PTR<const ObjectList> game_objects = MakeGlobalPtr<const ObjectList>();
    mutable MUTEX mutex_objects;

    STRING scene_name;
    BOOL is_started = false;
    DebugInfo debug_info = {};
    HASH_MAP<STRING, INT> layers;
    VECTOR<VIEW_PTR<GameObject>> GetSortedByLayer(const GLOBAL_PTR<const ObjectList>& snapshot);
public:
    void SetLayer(INT index, const STRING &layer);
    INT GetLayer(const STRING& layer);

    explicit Scene(MOVE_PLEASE STRING scene_name);
    VIRTUAL ~Scene() = default;
    const DebugInfo& GetDebugInfo() const;
    void Setup();
    VIRTUAL void Start();
    VIRTUAL void Update();
    VIRTUAL void Render();
    VIRTUAL void Finish();

    VIRTUAL RGBA GetBackgroundColor();

    VIRTUAL void UI();

    void AddGameObject(const GLOBAL_PTR<GameObject>& game_object);
    void DeleteGameObject(VIEW_PTR<GameObject> game_object);
    VIEW_PTR<RenderSystem> GetRenderSystem();

    static App& GetApp();
    LIST<VIEW_PTR<GameObject>> GetGameObjectByTags(const STRING& tag) const;

    template <typename TemplateComponent>
    LIST<VIEW_PTR<TemplateComponent>> GetComponents();

    GLOBAL_PTR<const ObjectList> Snapshot() const;

    STRING GetName() const;
    BOOL IsStarted() const;

    static DOUBLE GetDeltaTime();

    INT GetGameObjectSize() const;
};

template<typename TemplateComponent>
LIST<VIEW_PTR<TemplateComponent>> Scene::GetComponents() {
    LIST<VIEW_PTR<TemplateComponent>> result;

    const auto snapshot = Snapshot();
    for (const auto& game_object : *snapshot) {
        if (auto component = game_object->GetComponent<TemplateComponent>()) {
            result.push_back(component);
        }
    }

    return result;
}


#endif //FASTENGINE_SCENE_H
