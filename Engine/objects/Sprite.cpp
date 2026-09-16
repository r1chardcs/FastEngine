//
// Created by dlllibstdntc on 16.09.2026.
//

#include "Sprite.h"

#include "../../Toolkit/Debug/Logger.h"
#include "../components/Transform.h"

Sprite::Sprite(Texture texture) : texture(MOVE(texture)) {
    AddComponent<Transform>();
}

void Sprite::DrawWorld() {
    GameObject::DrawWorld();

    const auto transform = GetComponent<Transform>();
    if (!transform) {
        LOGWRN.Output("Sprite has no Transform component");
        return;
    }

    GetRenderSystem()->NewContext();
    GetRenderSystem()->Rotate(transform->Rotation());

    if (useSrcRect) {
        Render2D::DrawTexture(
            texture,
            srcRect,
            transform->Position().ToVec2(),
            transform->Size().ToVec2(),
            Brush::Solid(255, 255, 255, 255));
    } else {
        Render2D::DrawTexture(
            texture,
            transform->Position().ToVec2(),
            transform->Size().ToVec2(),
            Brush::Solid(255, 255, 255, 255));
    }

    GetRenderSystem()->StopContext();
}

void Sprite::SetSourceRect(const Recti& rect) {
    srcRect = rect;
    useSrcRect = true;
}

void Sprite::ClearSourceRect() {
    useSrcRect = false;
}

Texture& Sprite::GetTexture() {
    return texture;
}

