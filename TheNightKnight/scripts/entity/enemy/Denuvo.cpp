//
// Created by dlllibstdntc on 18.09.2026.
//

#include "Denuvo.h"

void Denuvo::Start() {
    EnemyEntity::Start();
}

void Denuvo::Move(FLOAT addX, FLOAT addY, FLOAT curspeed) {
    EnemyEntity::Move(addX, addY, curspeed);
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
