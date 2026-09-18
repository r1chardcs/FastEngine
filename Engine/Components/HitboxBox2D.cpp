//
// Created by dlllibstdntc on 17.09.2026.
//

#include "HitboxBox2D.h"

#include "Engine/App.h"
#include "Engine/GameObject.h"
#include "Engine/Render2D.h"
#include "Engine/Components/Transform.h"
#include "Toolkit/Debug/Logger.h"

Vec2f& HitboxBox2D::Size() {
    return size;
}

Vec2f& HitboxBox2D::Position() {
    return position;
}

Box2D& HitboxBox2D::Box() {
    return box;
}

BOOL& HitboxBox2D::IsSolid() {
    return solid;
}

void HitboxBox2D::Start() {
    Component::Start();

    transform = self->GetComponent<Transform>();

    if (!transform) {
        LOGWRN.Output("HitboxBox2D has no Transform component");
    }
}

void HitboxBox2D::Update() {
    Component::Update();

    if (!transform) {
        transform = self->GetComponent<Transform>();

        if (!transform) {
            LOGWRN.Output("HitboxBox2D has no Transform component");
            return;
        }
    }

    const Vec2f transform_pos = transform->Position().ToVec2();
    const Vec2f transform_size = transform->Size().ToVec2();

    const Vec2f hitbox_pos = {
        transform_pos.x + position.x,
        transform_pos.y + position.y
    };

    const Vec2f hitbox_size = {
        size.x > 0.0f ? size.x : transform_size.x,
        size.y > 0.0f ? size.y : transform_size.y
    };

    const float halfW = hitbox_size.x * 0.5f;
    const float halfH = hitbox_size.y * 0.5f;

    box = {
        hitbox_pos.x - halfW,
        hitbox_pos.y - halfH,
        hitbox_pos.x + halfW,
        hitbox_pos.y + halfH
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
            auto& transform_position = transform->Position();

            transform_position.x += pushX;
            transform_position.y += pushY;

            const Vec2f new_hitbox_pos = {
                transform_position.x + position.x,
                transform_position.y + position.y
            };

            box = {
                new_hitbox_pos.x - halfW,
                new_hitbox_pos.y - halfH,
                new_hitbox_pos.x + halfW,
                new_hitbox_pos.y + halfH
            };
        }
    }
}

void HitboxBox2D::Render(Type type) {
    if (type == Type::Pre) return;
    if (isDebug) {
        Component::Render();

        self->GetRenderSystem()->NewContext();

        Render2D::DrawBorder(
            box,
            {{1, 0, 0, 1}}
        );

        self->GetRenderSystem()->StopContext();
    }
}

BOOL HitboxBox2D::IsCollision(VIEW_PTR<GameObject> game_object) const {
    if (const auto hitbox = game_object->GetComponent<HitboxBox2D>()) {
        return IsCollision(hitbox);
    }

    return false;
}

BOOL HitboxBox2D::IsCollision(VIEW_PTR<HitboxBox2D> box2d) const {
    return IsCollision(&box2d->Box());
}

BOOL HitboxBox2D::IsCollision(VIEW_PTR<Box2D> box2d) const {
    return box.Contains(*box2d);
}
