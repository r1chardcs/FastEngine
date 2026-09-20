//
// Created by dlllibstdntc on 18.09.2026.
//

#include "Player.h"

#include "glfw3.h"
#include "Engine/App.h"
#include "Engine/Components/Sprite.h"
#include "Engine/Components/Transform.h"
#include "TheNightKnight/scenes/DieScene.h"
#include "Toolkit/Input.h"

Player::Player(): LivingEntity(20, 3) {
    const auto sprite = AddComponent<Sprite>();
    sprite->SetTexture(GetRenderSystem()->LoadTextureSync("assets/demons.png"));
    sprite->SetSourceRect({.x = 0, .y = 0, .width = 16, .height = 16});
}

void Player::AddItem(const GLOBAL_PTR<Item> &item) {
    this->items.push_back(item);
}

void Player::RemoveItem(VIEW_PTR<Item> item) {
    items.remove_if([&item](const GLOBAL_PTR<Item>& source) {
        return source.get() == item.operator->();
    });
}

void Player::SetWeapon(const GLOBAL_PTR<Weapon> &weapon_) {
    if (weapon) {
        weapon->Destroy();
    }

    this->weapon = weapon_;
}

void Player::Update() {
    LivingEntity::Update();

    if (Input::Down(Input::MoveUp)) {
        Move(0, 1, speed);
    }
    if (Input::Down(Input::MoveDown)) {
        Move(0, -1, speed);
    }
    if (Input::Down(Input::MoveRight)) {
        Move(1, 0, speed);
    }
    if (Input::Down(Input::MoveLeft)) {
        Move(-1, 0, speed);
    }

    UpdateWeapon();
    UpdateAttack();
}

void Player::DrawWorld() {
    LivingEntity::DrawWorld();
}

void Player::UpdateWeapon() {

}

void Player::UpdateAttack() {
    attackCooldown -= GetApp().GetDeltaTime();

    if (attackCooldown > 0.0f)
        return;

    if (GetApp().GetKey(GLFW_KEY_SPACE)) {
        auto vic = FindVictim(2.5);

        if (vic) {
            vic->Damage(3);
            attackCooldown = 1.0f;
        }
    }
}

VIEW_PTR<LivingEntity> Player::FindVictim(FLOAT radius) {
    const auto selfTransform = GetComponent<Transform>();
    if (!selfTransform)
        return nullptr;

    const auto selfPos = selfTransform->Position();

    const auto snapshot = App::GetInstance().GetScene()->Snapshot();

    for (const auto& gameObject : *snapshot) {
        if (gameObject.get() == this)
            continue;

        if (!gameObject->IsActive())
            continue;

        const auto living = dynamic_cast<LivingEntity*>(gameObject.get());
        if (!living)
            continue;

        const auto transform = living->GetComponent<Transform>();
        if (!transform)
            continue;

        if (transform->Position().DistanceTo(selfPos) <= radius) {
            return living;
        }
    }

    return nullptr;
}

void Player::Move(FLOAT addX, FLOAT addY, FLOAT curspeed) {
    LivingEntity::Move(addX, addY, curspeed);
    const auto sprite = GetComponent<Sprite>();
    if (addX > 0) {
        sprite->SetFlipX(false);
    }
    if (addX < 0) {
        sprite->SetFlipX(true);
    }
}

RGBA Player::GetHealthBarColor() {
    return {.r = 0, .g = 0.8, .b = 0, .a = 1};
}

void Player::Died() {
    LivingEntity::Died();
}
