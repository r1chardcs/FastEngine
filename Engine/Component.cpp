//
// Created by dlllibstdntc on 16.09.2026.
//

#include "Component.h"

Component::Component(VIEW_PTR<GameObject> self): self(self) {}

void Component::Start() {}

void Component::Update() {}

void Component::Render() {}

void Component::Shutdown() {
}
