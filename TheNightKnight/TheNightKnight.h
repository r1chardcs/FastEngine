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
#include "Engine/UIContext.h"

class TheNightKnight : public App {
    GLOBAL_PTR<Rect> player;
    GLOBAL_PTR<Rect> victim;
public:
    TheNightKnight()
        : App("The Night Knight") {
    }

    void UI(TypeEvent type) override {
        UIContext ctx(GetRenderSystem());
        const auto font = Render2D::GetFont("C:/Windows/Fonts/Arial.ttf", 32).res;
        char buf[32];
        snprintf(buf, sizeof(buf), "R-FPS: %.0f | S-FPS: %.0f", GetRawFPS(), GetFPS());

        ctx.Text(Layout::UP_LEFT, font, buf, {5 ,5}, {1, 1, 1, 1}, 0.5);
    }

    void Start() override {
        player = MakeGlobalPtr<Rect>();
        victim = MakeGlobalPtr<Rect>();

        const auto transform = player->GetComponent<Transform>();
        player->AddComponent<HitboxBox2D>();
        victim->AddComponent<HitboxBox2D>();

        victim->GetComponent<Transform>()->Size().x = 0.1;
        victim->GetComponent<Transform>()->Size().y = 0.1;
        victim->SetColor({{1,0,0,1}});

        transform->Size().x = 0.1;
        transform->Size().y = 0.1;
        transform->Position().y = 0.3;
        transform->Position().y = 0.3;
        player->SetColor({1,0,0,1});
        AddGameObject(player);
        AddGameObject(victim);
    }

    void Update() override {
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
