//
// Created by dlllibstdntc on 16.09.2026.
//

#include "Object.h"

#include "App.h"
#include "../Toolkit/Debug/Logger.h"

void GameObject::AddTag(STRING tag) {
    if (tag.empty()) {
        LOGWRN.Output("Invalid tag: %s", tag.c_str());
        return;
    }
    this->tags.emplace_back(MOVE(tag));
}

void GameObject::DeleteTag(const STRING &tag) {
    tags.remove(tag);
}

LIST<STRING> GameObject::GetTags() {
    return tags;
}

VIEW_PTR<RenderSystem> GameObject::GetRenderSystem() {
    return App::GetInstance().GetRenderSystem();
}

void GameObject::SetActive(const BOOL value) {
    isActive = value;
}

BOOL GameObject::IsActive() const {
    return isActive;
}
