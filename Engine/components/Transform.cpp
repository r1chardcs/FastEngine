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

Vec3f & Transform::Size() {
    return size;
}
