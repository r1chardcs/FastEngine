//
// Created by dlllibstdntc on 17.09.2026.
//

#include "HitboxBox2D.h"

#include "Engine/App.h"
#include "Engine/Object.h"
#include "Engine/Render2D.h"
#include "Engine/components/Transform.h"
#include "Toolkit/Debug/Logger.h"


Vec2f & HitboxBox2D::Size() {
    return size;
}

Box2D & HitboxBox2D::Box() {
    return box;
}

BOOL & HitboxBox2D::IsSolid() {
    return solid;
}

void HitboxBox2D::Start() {
    Component::Start();
}

void HitboxBox2D::Update() {
    Component::Update();
    transform = self->GetComponent<Transform>();

    if (!transform) {
        LOGWRN.Output("HitboxBox2D has no Transform component");
        return;
    }

    auto transform_pos = transform->Position();
    auto transform_size = transform->Size();
    auto custom_size = size;



    const float halfW = transform_size.x * 0.5f + size.x;
    const float halfH = transform_size.y * 0.5f + size.y;

    box = {
        transform_pos.x - halfW,
        transform_pos.y - halfH,
        transform_pos.x + halfW,
        transform_pos.y + halfH
    };

    if (!solid) {
        return;
    }

    for (const auto& other : self->GetApp().GetComponents<HitboxBox2D>()) {
        if (other == this || !other->IsSolid()) {
            continue;
        }

        float pushX = 0.0f;
        float pushY = 0.0f;

        if (box.Overlap(other->Box(), pushX, pushY)) {
            transform_pos.x += pushX;
            transform_pos.y += pushY;

            transform->Position() = transform_pos;

            box = {
                transform_pos.x - halfW, transform_pos.y - halfH,
                transform_pos.x + halfW, transform_pos.y + halfH
            };
        }
    }
}

void HitboxBox2D::Render() {
    Component::Render();

    self->GetRenderSystem()->NewContext();
    Render2D::DrawBorder(box, {{1, 0, 0, 1}});
    self->GetRenderSystem()->StopContext();
}

BOOL HitboxBox2D::IsCollision(VIEW_PTR<GameObject> game_object) const {
    if (const auto hitbox = game_object->GetComponent<HitboxBox2D>()) {
        if (IsCollision(hitbox)) {
            return true;
        }
    }
    return false;
}

BOOL HitboxBox2D::IsCollision(VIEW_PTR<HitboxBox2D> box2d) const {
    return IsCollision(&box2d->Box());
}

BOOL HitboxBox2D::IsCollision(VIEW_PTR<Box2D> box2d) const {
    return box.Contains(*box2d);
}

