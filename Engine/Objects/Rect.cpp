//
// Created by dlllibstdntc on 16.09.2026.
//

#include "Rect.h"

#include "../Render2D.h"
#include  "../Components/Transform.h"

Rect::Rect() {
    AddComponent<Transform>();
}

void Rect::SetColor(const toolkit::Brush &color_) {
    this->color = color_;
}

toolkit::Brush Rect::GetColor() {
    return color;
}

void Rect::DrawWorld() {
    GameObject::DrawWorld();

    const auto transform = GetComponent<Transform>();

    Render2D::DrawRect(
      transform->HalfPos().ToVec2(),
      transform->Size().ToVec2(),
      color);
}
