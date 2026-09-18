//
// Created by dlllibstdntc on 17.09.2026.
//

#include "Sprite.h"

#include "Transform.h"
#include "../../Toolkit/Debug/Logger.h"

void Sprite::SetTexture(const NIL<Texture> &texture_) {
    this->texture = texture_;
}

void Sprite::Render() {
    const auto transform = self->GetComponent<Transform>();
    if (!transform) {
        LOGWRN.Output("Sprite has no Transform component");
        return;
    }

    if (not texture.has_value()) {
        LOGWRN.Output("Texture is null");
        return;
    }

    self->GetRenderSystem()->NewContext();
    self->GetRenderSystem()->Rotate(transform->Rotation());


    if (useSrcRect) {
        Render2D::DrawTexture(
            texture.value(),
            srcRect,
            transform->Position().ToVec2(),
            transform->Size().ToVec2(),
            Brush::Solid(255, 255, 255, 255));
    } else {
        Render2D::DrawTexture(
            texture.value(),
            transform->Position().ToVec2(),
            transform->Size().ToVec2(),
            Brush::Solid(255, 255, 255, 255));
    }


    self->GetRenderSystem()->StopContext();
}

void Sprite::SetSourceRect(const Recti& rect) {
    srcRect = rect;
    useSrcRect = true;
}

void Sprite::ClearSourceRect() {
    useSrcRect = false;
}