//
// Created by dlllibstdntc on 22.09.2026.
//

#ifndef FASTENGINE_MAINSCENE_H
#define FASTENGINE_MAINSCENE_H

#include <Engine/Scene.h>
#include <Engine/UIContext.h>


class MainScene : public Scene {
public:
    explicit MainScene()
        : Scene("MainScene") {
    }

    void UI() override {
        UIContext ctx(GetRenderSystem());
        ctx.Text(
          Layout::CENTER, Render2D::GetFont("C:/Windows/Fonts/Arial.ttf", 32).res.get(), "Hello, World!", {}, {1, 1, 1, 1}, 0.5
        );
    }
};

#endif //FASTENGINE_MAINSCENE_H
