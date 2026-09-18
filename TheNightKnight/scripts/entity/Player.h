//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_PLAYER_H
#define FASTENGINE_PLAYER_H

#include "LivingEntity.h"

class Player : public LivingEntity {
public:
    Player();

    void Update() override;
    void DrawWorld() override;

    void Move(FLOAT addX, FLOAT addY, FLOAT curspeed) override;
    RGBA GetHealthBarColor() override;
};


#endif //FASTENGINE_PLAYER_H
