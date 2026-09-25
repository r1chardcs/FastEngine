//
// Created by dlllibstdntc on 16.09.2026.
//

#include "Transform.h"
#include "../GameObject.h"

void Transform::Render() {
    Component::Render();
    const auto transform = self->GetComponent<Transform>();
    self->GetRenderSystem()->Rotate(transform->Rotation());

}

toolkit::Quat & Transform::Rotation() {
    return rotation;
}

toolkit::Vec3f & Transform::Position() {
    return position;
}

toolkit::Vec3f Transform::HalfPos() const {
    return {
        position.x - size.x * 0.5f,
        position.y - size.y * 0.5f,
        position.z - size.z * 0.5f,
    };
}

toolkit::Vec3f & Transform::Size() {
    return size;
}
