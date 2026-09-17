//
// Created by dlllibstdntc on 16.09.2026.
//

#include "Rect.h"

#include "../Render2D.h"

Rect::Rect() {
    AddComponent<Transform>();
}

void Rect::SetColor(const Brush &color_) {
    this->color = color_;
}

Brush Rect::GetColor() {
    return color;
}

void Rect::DrawWorld() {
    GameObject::DrawWorld();

    GetRenderSystem()->NewContext();
    const auto transform = GetComponent<Transform>();
    printf("Transform %f %f %f %f\n", transform->Position().x, transform->Position().y, transform->Size().x, transform->Size().y);

    GetRenderSystem()->Rotate(transform->Rotation());
    Render2D::DrawRect(
        transform->Position().ToVec2(),
        transform->Size().ToVec2(),
        color);

    GetRenderSystem()->StopContext();
}
