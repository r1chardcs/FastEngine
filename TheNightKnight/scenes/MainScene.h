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
#include "TheNightKnight/scripts/entity/health/HealthEntity.h"
#include "TheNightKnight/scripts/map/NatureMapGenerator.h"
#include "TheNightKnight/scripts/weapon/sword/OxidativeSword.h"
#include "Toolkit/Debug/MemoryProfiler.h"

class MainScene : public Scene {
public:
    GLOBAL_PTR<Player> player;
    GLOBAL_PTR<Denuvo> denuvo;

    MainScene()
        : Scene("MainScene") {
    }

    RGBA GetBackgroundColor() override {
        return RGBA(0.17f,0.17f,0.17f);
    }

    void Start() override {
        player = MakeGlobalPtr<Player>();

        AddGameObject(player);
        AddGameObject(MakeGlobalPtr<HealthEntity>());
        AddGameObject(MakeGlobalPtr<FPSOverlay>());
        NatureMapGenerator::generate(50);
    }
};


#endif //FASTENGINE_MAINSCENE_H
