//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_OBJECT_H
#define FASTENGINE_OBJECT_H

#include "Component.h"
#include "Platform.h"

class GameObject {
    LIST<STRING> tags;
    VECTOR<SELF_PTR<Component>> components;
public:
    void AddTag(MOVE_PLEASE STRING tag);
    void DeleteTag(const STRING& tag);

    LIST<STRING> GetTags();

    template <typename TemplateComponent>
    VIEW_PTR<TemplateComponent> AddComponent() {
        static_assert(std::is_base_of_v<Component, TemplateComponent>,
                      "TemplateComponent must derive from Component");

        auto component = MakeSelfPtr<TemplateComponent>();
        const auto raw_ptr = component.get();
        components.push_back(MOVE(component));
        return raw_ptr;
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
};


#endif //FASTENGINE_OBJECT_H
