//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_OBJECT_H
#define FASTENGINE_OBJECT_H

#include "Component.h"
#include "Platform.h"
#include "RenderSystem.h"

class GameObject {
    LIST<STRING> tags;
    VECTOR<SELF_PTR<Component>> components;
    BOOL isActive = true;
public:
    virtual ~GameObject() = default;

    void AddTag(MOVE_PLEASE STRING tag);
    void DeleteTag(const STRING& tag);

    LIST<STRING> GetTags();

    VIRTUAL VOID Start() {}
    VIRTUAL VOID Shutdown() {}
    VIRTUAL VOID Update() {}

    VIRTUAL VOID DrawWorld() {}
    VIRTUAL VOID DrawUI() {}

    VIEW_PTR<RenderSystem> GetRenderSystem();

    template <typename TemplateComponent>
    VIEW_PTR<TemplateComponent> AddComponent() {
        static_assert(std::is_base_of_v<Component, TemplateComponent>,
                      "TemplateComponent must derive from Component");

        auto component = MakeSelfPtr<TemplateComponent>(TemplateComponent(this));
        component->Start();
        const auto raw_ptr = component.get();
        components.push_back(MOVE(component));
        return raw_ptr;
    }

    VECTOR<VIEW_PTR<Component>> GetComponents() const {
        VECTOR<VIEW_PTR<Component>> array;
        for (const auto &component : components) {
            if (component) array.push_back(component.get());
        }

        return array;
    }

    template <typename TemplateComponent>
    VIEW_PTR<TemplateComponent> GetComponent() {
        static_assert(std::is_base_of_v<Component, TemplateComponent>,
                      "TemplateComponent must derive from Component");

        for (const auto &component : components) {
            if (component && component->GetClassID() == TemplateComponent::StaticClassID())
                return static_cast<TemplateComponent*>(component.get());
        }
        return nullptr;
    }

    void SetActive(BOOL value);
    BOOL IsActive() const;

    App& GetApp();
};


#endif //FASTENGINE_OBJECT_H
