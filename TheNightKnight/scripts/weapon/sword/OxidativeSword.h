//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_OXIDATIVESWORD_H
#define FASTENGINE_OXIDATIVESWORD_H

#include "../Weapon.h"
#include "Engine/Components/Sprite.h"

class OxidativeSword : public Weapon {
public:
    OxidativeSword() {
        auto sprite = AddComponent<Sprite>();
        sprite->SetTexture(
            GetRenderSystem()->LoadTextureSync("assets/weapons.png")
        );

        sprite->SetSourceRect({0, 0, 16, 16});
    }

    void DrawWorld() override {}
};


#endif //FASTENGINE_OXIDATIVESWORD_H
