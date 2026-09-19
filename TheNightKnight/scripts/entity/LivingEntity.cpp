//
// Created by dlllibstdntc on 18.09.2026.
//

#include "LivingEntity.h"

#include "Engine/components/HitboxBox2D.h"
#include "Engine/Components/Transform.h"

LivingEntity::LivingEntity(INT maxHealth, FLOAT speed): maxHealth(maxHealth), speed(speed), health(maxHealth) {
    transform = AddComponent<Transform>();
    transform->Size().x = 1;
    transform->Size().y = 1;

    const auto hitbox = AddComponent<HitboxBox2D>();
    hitbox->Position() = {0.0f, -0.42f};
    hitbox->Size() = {0.58f, 0.26f};
}

void LivingEntity::DrawWorld() {
    GameObject::DrawWorld();

    DrawHealthBar(GetHealthBarColor());
}
void LivingEntity::Move(
    const FLOAT addX,
    const FLOAT addY,
    const FLOAT curspeed) {
    if (!transform) {
        LOGERR.Output("LivingEntity::Move called without Transform component\n");
        return;
    }
    transform->Position().x += addX * curspeed * GetDeltaTime();
    transform->Position().y += addY * curspeed * GetDeltaTime();
}

void LivingEntity::MoveTo(const Vec2f to) {
    if (!transform) {
        LOGERR.Output("LivingEntity::MoveTo called without Transform component\n");
        return;
    }

    const auto pos = transform->Position();
    const FLOAT dx = to.x - pos.x;
    const FLOAT dy = to.y - pos.y;

    const FLOAT distance = std::sqrt(dx * dx + dy * dy);

    constexpr FLOAT epsilon = 0.001f;
    if (distance < epsilon) {
        return;
    }

    const FLOAT dirX = dx / distance;
    const FLOAT dirY = dy / distance;

    if (const FLOAT step = speed * static_cast<FLOAT>(GetDeltaTime()); step >= distance) {
        transform->Position().x = to.x;
        transform->Position().y = to.y;
        return;
    }

    Move(dirX, dirY, speed);
}

void LivingEntity::Teleport(FLOAT x, FLOAT y) {
    if (!transform) {
        AddComponent<Transform>();
    }

    transform->Position().x = x;
    transform->Position().y = y;
}

void LivingEntity::DrawHealthBar(const RGBA &color) {
    const auto pos = transform->Position();
    const auto halfPos = transform->HalfPos();

    constexpr FLOAT barWidth = 0.6f;
    constexpr FLOAT barHeight = 0.04f;

    const FLOAT ratio = maxHealth > 0
        ? static_cast<FLOAT>(health) / static_cast<FLOAT>(maxHealth)
        : 0.0f;

    const FLOAT centerX = pos.x;
    const Vec2f barPos = {centerX - barWidth / 2.0f, halfPos.y + 1.02f};

    Render2D::DrawRect(
        barPos, {barWidth, barHeight},
        {{.r = 0.2f, .g = 0.2f, .b = 0.2f, .a = 1}}
    );

    Render2D::DrawRect(
        barPos, {barWidth * ratio, barHeight},
        {{color}}
    );
}

RGBA LivingEntity::GetHealthBarColor() {
    return {1, 0, 0, 1};
}

BOOL LivingEntity::IsAlive() {
    return health > 0;
}

BOOL LivingEntity::Damage(INT damage) {
    if (damage <= 0) return IsAlive();

    health -= damage;
    if (health <= 0) {
        health = 0;
        Died();
        return false;
    }
    return true;
}

void LivingEntity::Died() {
    Destroy();
}

INT LivingEntity::GetMaxHealth() const {
    return maxHealth;
}

INT LivingEntity::GetHealth() const {
    return health;
}
