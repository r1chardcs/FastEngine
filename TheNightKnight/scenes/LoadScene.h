//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_LOADSCENE_H
#define FASTENGINE_LOADSCENE_H

#include <Engine/Scene.h>

#include "Engine/UIContext.h"

class LoadScene : public Scene {
public:
    explicit LoadScene()
        : Scene("Load Scene") {
    }

    void UI() override {
        UIContext ctx(GetRenderSystem());

        auto [texture, err] = Render2D::GetTexture("assets/gameicon.png");

        ctx.Image(
            Layout::CENTER,
            texture.get(),
            Vec2i{0, -50},
            Vec2i{640, 640},
            {.r = 0, .g = 0, .b = 0, .a = 0}, 0.5
        );

        ctx.Text(
            Layout::DOWN_RIGHT,
            Render2D::GetFont("assets/roboto.ttf", 32).res.get(),
            "make by megantcs",
            {5, 5},
            {1, 1, 1, 1},
            0.5
        );

        ctx.Text(
            Layout::CENTER,
            Render2D::GetFont("assets/roboto.ttf", 32).res.get(),
            "The Night Knight",
            {0, 130},
            {1, 1, 1, 1},
            1
        );
    }

    RGBA GetBackgroundColor() override {
        return {.r = 0, .g = 0, .b = 0, .a = 1};
    }
};


#endif //FASTENGINE_LOADSCENE_H
