//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_LIVINGENTITY_H
#define FASTENGINE_LIVINGENTITY_H

#include <Engine/GameObject.h>

class LivingEntity : public GameObject {
protected:
    INT health;
    INT maxHealth;
    FLOAT speed;
public:
    LivingEntity(INT maxHealth, FLOAT speed);

    void DrawWorld() override;

    VIRTUAL void Move(FLOAT addX, FLOAT addY, FLOAT curspeed);
    VIRTUAL void MoveTo(Vec2f to);

    VIRTUAL void Teleport(FLOAT x, FLOAT y);

    VIRTUAL void DrawHealthBar(const RGBA &color);
    VIRTUAL RGBA GetHealthBarColor();

    VIRTUAL BOOL IsAlive();
    VIRTUAL BOOL Damage(INT damage);
    VIRTUAL void Died();

    INT GetMaxHealth() const;

    INT GetHealth() const;
};


#endif //FASTENGINE_LIVINGENTITY_H
