//
// Created by dlllibstdntc on 16.09.2026.
//

#include "GameObject.h"

#include <algorithm>

#include "App.h"
#include "../Toolkit/Debug/Logger.h"
#include "Components/Transform.h"

void GameObject::SetLayer(STRING layer_name) {
    this->layer = MOVE(layer_name);
}

STRING GameObject::GetLayer() {
    return layer;
}

void GameObject::AddTag(STRING tag) {
    if (tag.empty()) {
        toolkit::LOGWRN.Output("Invalid tag: %s", tag.c_str());
        return;
    }
    this->tags.emplace_back(MOVE(tag));
}

void GameObject::DeleteTag(const STRING &tag) {
    tags.remove(tag);
}

const LIST<STRING> & GameObject::GetTags() const {
    return tags;
}

void GameObject::Start() {
}

void GameObject::Shutdown() {
}

void GameObject::Update() {
    for (const auto snapshot = children; const auto &child : snapshot) {
        if (child && child->IsActive()) {
            child->Update();
        }
    }
}

void GameObject::DrawWorld() {
    for (const auto snapshot = children; const auto &child : snapshot) {
        if (child && child->IsActive()) {
            child->DrawWorld();
        }
    }
}

void GameObject::DrawUI() {
    for (const auto snapshot = children; const auto &child : snapshot) {
        if (child && child->IsActive()) {
            child->DrawUI();
        }
    }
}

VIEW_PTR<GameObject> GameObject::AddChild(const GLOBAL_PTR<GameObject> &child) {
    if (!child) {
        toolkit::LOGWRN.Output("AddChild called with null child\n");
        return nullptr;
    }

    child->parent = this;
    const auto raw_ptr = child.get();

    children.push_back(child);
    child->Start();

    return raw_ptr;
}

void GameObject::RemoveChild(VIEW_PTR<GameObject> child) {
    std::erase_if(children,
                  [child](const GLOBAL_PTR<GameObject> &c) { return c.get() == child; });
}

VIEW_PTR<GameObject> GameObject::GetParent() const {
    return parent;
}

VECTOR<VIEW_PTR<GameObject>> GameObject::GetChildren() const {
    VECTOR<VIEW_PTR<GameObject>> result;
    for (const auto &child : children) {
        if (child) result.push_back(child.get());
    }
    return result;
}

void GameObject::Destroy() {
    if (parent) {
        parent->RemoveChild(this);
        return;
    }
    GetApp().GetScene()->DeleteGameObject(this);
}

DOUBLE GameObject::GetDeltaTime() {
    return GetRenderSystem()->GetDeltaTime();
}

VIEW_PTR<RenderSystem> GameObject::GetRenderSystem() {
    return GetApp().GetRenderSystem();
}

void GameObject::RebuildComponentsCache() {
    componentsCache.clear();
    componentsCache.reserve(components.size());
    for (const auto &component : components) {
        if (component) componentsCache.push_back(component.get());
    }
}

FLOAT GameObject::GetSortY() {
    if (const auto transform = GetComponent<Transform>()) {
        return transform->Position().y;
    }
    return 0.0f;
}

void GameObject::SetActive(const BOOL value) {
    isActive = value;
}

BOOL GameObject::IsActive() const {
    return isActive;
}

App& GameObject::GetApp() {
    return App::GetInstance();
}
