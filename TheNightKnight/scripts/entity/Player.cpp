//
// Created by dlllibstdntc on 18.09.2026.
//

#include "Player.h"

#include "glfw3.h"
#include "Engine/App.h"
#include "Engine/Components/Sprite.h"
#include "Engine/Components/Transform.h"

Player::Player(): LivingEntity(20, 3) {
    const auto sprite = AddComponent<Sprite>();
    sprite->SetTexture(GetRenderSystem()->LoadTextureSync("assets/players.png"));
    sprite->SetSourceRect({.x = 0, .y = 0, .width = 16, .height = 16});
}

void Player::Update() {
    LivingEntity::Update();

    if (GetApp().GetKey(GLFW_KEY_W)) {
        Move(0, 1, speed);
    }
    if (GetApp().GetKey(GLFW_KEY_S)) {
        Move(0, -1, speed);
    }
    if (GetApp().GetKey(GLFW_KEY_D)) {
        Move(1, 0, speed);
    }
    if (GetApp().GetKey(GLFW_KEY_A)) {
        Move(-1, 0, speed);
    }


}

void Player::DrawWorld() {
    LivingEntity::DrawWorld();
}

void Player::Move(FLOAT addX, FLOAT addY, FLOAT curspeed) {
    LivingEntity::Move(addX, addY, curspeed);
    const auto sprite = GetComponent<Sprite>();

    if (addY > 0) {
        sprite->SetSourceRect(Recti::CalculateStepRect(192, 64, 16, 8));
        sprite->SetFlipX(false);
    }
    if (addY < 0) {
        sprite->SetSourceRect(Recti::CalculateStepRect(192, 64, 16, 0));
        sprite->SetFlipX(false);
    }
    if (addX > 0) {
        sprite->SetSourceRect(Recti::CalculateStepRect(192, 64, 16, 4));
        sprite->SetFlipX(true);
    }
    if (addX < 0) {
        sprite->SetSourceRect(Recti::CalculateStepRect(192, 64, 16, 4));
        sprite->SetFlipX(false);
    }
}

RGBA Player::GetHealthBarColor() {
    return {.r = 0, .g = 0.8, .b = 0, .a = 1};
}
