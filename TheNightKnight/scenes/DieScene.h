//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_DIESCENE_H
#define FASTENGINE_DIESCENE_H

#include <Engine/Scene.h>

#include "Engine/UIContext.h"

class DieScene : public Scene {
public:
    DieScene()
        : Scene("DieScene") {
    }

    void UI() override {
        UIContext ctx(GetRenderSystem());
        ctx.Text(Layout::CENTER, Render2D::GetFont("C:/Windows/Fonts/Arial.ttf", 32).res.get(), "You Lost", {5, 5}, {1, 1, 1, 1}, 0.5);
    }
};


#endif //FASTENGINE_DIESCENE_H
