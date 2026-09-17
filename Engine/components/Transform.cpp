//
// Created by dlllibstdntc on 16.09.2026.
//

#include "Transform.h"

Quat & Transform::Rotation() {
    return rotation;
}

Vec3f & Transform::Position() {
    return position;
}

Vec3f Transform::HalfPos() const {
    return {
        position.x - size.x * 0.5f,
        position.y - size.y * 0.5f,
        position.z - size.z * 0.5f,
    };
}

Vec3f & Transform::Size() {
    return size;
}
