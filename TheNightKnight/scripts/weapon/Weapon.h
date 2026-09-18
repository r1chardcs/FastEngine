//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_WEAPON_H
#define FASTENGINE_WEAPON_H

#include <Engine/GameObject.h>

#include "Engine/Components/HitboxBox2D.h"
#include "Engine/Components/Transform.h"

class Weapon : public GameObject {
public:
    Weapon() {
        const auto transform = AddComponent<Transform>();
        transform->Size().x = 1;
        transform->Size().y = 1;

        const auto aabb = AddComponent<HitboxBox2D>();
        aabb->IsSolid() = false;
    }
};


#endif //FASTENGINE_WEAPON_H
