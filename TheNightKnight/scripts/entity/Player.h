//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_PLAYER_H
#define FASTENGINE_PLAYER_H

#include "LivingEntity.h"
#include "TheNightKnight/scripts/weapon/Weapon.h"

class Item;

class Player : public LivingEntity {
    GLOBAL_PTR<Weapon> weapon = nullptr;
    FLOAT attackCooldown = 0.0f;
    LIST<GLOBAL_PTR<Item>> items;

public:
    Player();

    void AddItem(const GLOBAL_PTR<Item>& item);
    void RemoveItem(VIEW_PTR<Item> item);

    void SetWeapon(const GLOBAL_PTR<Weapon> &weapon_);

    void Update() override;
    void DrawWorld() override;

    void UpdateWeapon();
    void UpdateAttack();
    VIEW_PTR<LivingEntity> FindVictim(float rad);
    void Move(FLOAT addX, FLOAT addY, FLOAT curspeed) override;
    RGBA GetHealthBarColor() override;

    void Died() override;
};


#endif //FASTENGINE_PLAYER_H
