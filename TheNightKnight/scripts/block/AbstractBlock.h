//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_ABSTRACTBLOCK_H
#define FASTENGINE_ABSTRACTBLOCK_H

#include <Engine/GameObject.h>

#include "Engine/components/HitboxBox2D.h"
#include "Engine/Components/Transform.h"

class AbstractBlock : public GameObject
{
public:
    AbstractBlock();
    void Start() override;

    FLOAT GetSortY() override {
        if (const auto hitbox = GetComponent<HitboxBox2D>()) {
            return hitbox->Box().minY;
        }
        return GameObject::GetSortY();
    }
};


#endif //FASTENGINE_ABSTRACTBLOCK_H
