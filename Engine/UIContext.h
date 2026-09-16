//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_UICONTEXT_H
#define FASTENGINE_UICONTEXT_H

#include "RenderSystem.h"
#include  "../Toolkit/Color.h"
#include "../Toolkit/Vector.h"

class UIContext {
    VIEW_PTR<RenderSystem> render_system;
public:
    UIContext(VIEW_PTR<RenderSystem> render_system);

    void DrawCircle(const Vec2f &pos, const Vec2f &size, const Brush& color,
        bool fill = true);

    void DrawBorder(const Vec2f &pos, const Vec2f &size, const Brush& color);
    void DrawRect(const Vec2f &pos, const Vec2f &size, const Brush& color);
    void DrawLine(FLOAT y, FLOAT minX, FLOAT maxX, const Brush &color);
};


#endif //FASTENGINE_UICONTEXT_H
