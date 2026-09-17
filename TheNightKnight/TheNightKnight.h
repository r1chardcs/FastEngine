//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_THENIGHTKNIGHT_H
#define FASTENGINE_THENIGHTKNIGHT_H

#include <Engine/App.h>
#include <Engine/objects/Rect.h>
#include <Engine/components/Transform.h>
#include <Engine/components/HitboxBox2D.h>

#include "glfw3.h"

class TheNightKnight : public App {
    GLOBAL_PTR<Rect> player;
public:
    TheNightKnight()
        : App("The Night Knight") {
    }

    void Start() override {
        player = MakeGlobalPtr<Rect>();
        const auto transform = player->GetComponent<Transform>();
        player->AddComponent<HitboxBox2D>();

        transform->Size().x = 0.1;
        transform->Size().y = 0.1;
        player->SetColor({1,0,0,1});
        AddGameObject(player);
    }

    void Update() override {
        ExecuteInRenderThread([](auto s) {
           Render2D::DrawBorder({0, 0, 0.5, 0.5}, {{1, 0, 0, 1}});
        });

        const auto transform = player->GetComponent<Transform>();
        if (GetKey(GLFW_KEY_W)) {
            transform->Position().y += 0.5 * GetDeltaTime();
        }
        if (GetKey(GLFW_KEY_S)) {
            transform->Position().y -= 0.5 * GetDeltaTime();
        }
        if (GetKey(GLFW_KEY_D)) {
            transform->Position().x += 0.5 * GetDeltaTime();
        }
        if (GetKey(GLFW_KEY_A)) {
            transform->Position().x -= 0.5 * GetDeltaTime();
        }
    }
};


#endif //FASTENGINE_THENIGHTKNIGHT_H
