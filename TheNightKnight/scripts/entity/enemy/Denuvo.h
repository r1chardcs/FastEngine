//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_DENUVO_H
#define FASTENGINE_DENUVO_H

#include "../EnemyEntity.h"
#include "Engine/Components/Sprite.h"
#include "TheNightKnight/scripts/entity/ai/MeleeAIStates.h"

class Denuvo : public EnemyEntity
{
public:
    Denuvo()
        : EnemyEntity(20, 2, MakeSelfPtr<MeleeSearchState>(), NULLPTR) {
        const auto sprite = AddComponent<Sprite>();
        sprite->SetTexture(GetRenderSystem()->LoadTextureSync("assets/players.png"));
        sprite->SetSourceRect({.x = 0, .y = 0, .width = 16, .height = 16});

    }

    void Start() override;
    void Move(FLOAT addX, FLOAT addY, FLOAT curspeed) override;

    FLOAT GetSearchRadius() override { return 15.0f * GetSpecifications()->celerity; }
    FLOAT GetDamage() override { return 1.5f * GetSpecifications()->strengthening; }
    FLOAT GetAttackRange() override { return 1.2f; }
};

#endif //FASTENGINE_DENUVO_H
