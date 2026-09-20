//
// Created by dlllibstdntc on 20.09.2026.
//

#include "HealthEntity.h"

#include "Engine/Components/HitboxBox2D.h"
#include "Engine/Components/Sprite.h"

HealthEntity::HealthEntity(): LivingEntity(20, 3) {
    AddTag("health");

    const auto position = GetComponent<Transform>();
    position->Size().x = 5;
    position->Size().y = 5;

    const auto hb = GetComponent<HitboxBox2D>();

    hb->NoMove() = true;
    hb->Position() = {0.0f, -1.22f};
    hb->Size() = {3.58f, 1};
    hb->isDebug = true;

    const auto sprite = AddComponent<Sprite>();
    sprite->SetTexture(GetRenderSystem()->LoadTextureSync("assets/health.png"));
    sprite->SetSourceRect({.x = 0, .y = 0, .width = 100, .height = 100});
}

RGBA HealthEntity::GetHealthBarColor() {
    return {0, 1, 0, 1};
}

void HealthEntity::DrawHealthBar(const RGBA &color) {
    const auto pos = transform->Position();
    const auto halfPos = transform->HalfPos();

    constexpr FLOAT barWidth = 0.8f;
    constexpr FLOAT barHeight = 0.15f;

    const FLOAT ratio = maxHealth > 0
        ? static_cast<FLOAT>(health) / static_cast<FLOAT>(maxHealth)
        : 0.0f;

    const FLOAT centerX = pos.x;
    const Vec2f barPos = {centerX - barWidth / 2.0f, halfPos.y + 4.32f};

    Render2D::DrawRect(
        barPos, {barWidth, barHeight},
        {{.r = 0.2f, .g = 0.2f, .b = 0.2f, .a = 1}}
    );

    Render2D::DrawRect(
        barPos, {barWidth * ratio, barHeight},
        {{color}}
    );
}
