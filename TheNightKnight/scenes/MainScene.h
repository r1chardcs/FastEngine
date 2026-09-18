//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_MAINSCENE_H
#define FASTENGINE_MAINSCENE_H

#include <Engine/Scene.h>
#include "../scripts/entity/Player.h"
#include "../scripts/FPSOverlay.h"

#include "Engine/components/HitboxBox2D.h"
#include "Engine/Components/Sprite.h"
#include "Engine/Components/Transform.h"
#include "../scripts/map/MapGenerator.h"
#include "TheNightKnight/scripts/entity/enemy/Denuvo.h"

class MainScene : public Scene {
public:
    GLOBAL_PTR<Player> player;
    GLOBAL_PTR<Denuvo> denuvo;

    MainScene()
        : Scene("MainScene") {
    }

    void Start() override {
        player = MakeGlobalPtr<Player>();
        denuvo = MakeGlobalPtr<Denuvo>();

        AddGameObject(player);
        AddGameObject(denuvo);
        AddGameObject(MakeGlobalPtr<FPSOverlay>());
        denuvo->Teleport(5, 5);
        auto map = MapGenerator::CreateMap(10, 10, {{1, 1, 1, 1}});
        for (const auto &color_block : map) {
            // AddGameObject(color_block);
        }

    }
};


#endif //FASTENGINE_MAINSCENE_H
