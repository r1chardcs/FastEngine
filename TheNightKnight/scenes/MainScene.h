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
#include "TheNightKnight/scripts/weapon/sword/OxidativeSword.h"

class MainScene : public Scene {
public:
    GLOBAL_PTR<Player> player;
    GLOBAL_PTR<Denuvo> denuvo;

    MainScene()
        : Scene("MainScene") {
    }

    void Start() override {
        static auto obj = MakeGlobalPtr<GameObject>();
        obj->AddComponent<Transform>();
        obj->AddComponent<HitboxBox2D>();
        auto sprite = obj->AddComponent<Sprite>();
        sprite->SetTexture(GetRenderSystem()->LoadTextureSync("assets/players.png"));

        AddGameObject(obj);

        player = MakeGlobalPtr<Player>();
        denuvo = MakeGlobalPtr<Denuvo>();

        AddGameObject(player);
        AddGameObject(denuvo);
        AddGameObject(MakeGlobalPtr<FPSOverlay>());
        denuvo->Teleport(7, 7);

    }
};


#endif //FASTENGINE_MAINSCENE_H
