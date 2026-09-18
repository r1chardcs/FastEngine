//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_UICONTEXT_H
#define FASTENGINE_UICONTEXT_H

#include "RenderSystem.h"
#include "../Toolkit/Color.h"
#include "../Toolkit/Vector.h"
#include "Render2D.h"

enum class Layout {
    CENTER_LEFT,
    CENTER_RIGHT,
    CENTER,

    UP_LEFT,
    UP_RIGHT,
    UP_CENTER,

    DOWN_LEFT,
    DOWN_RIGHT,
    DOWN_CENTER
};

class UIContext {
    VIEW_PTR<RenderSystem> render_system;

    Vec2i GetLayoutPosition(
        Layout layout,
        Vec2i pos,
        Vec2i size
    ) const;

public:
    explicit UIContext(VIEW_PTR<RenderSystem> render_system)
        : render_system(render_system) {
    }

    Vec2i GetWindowSize() const;

    void Text(
        Layout layout,
        VIEW_PTR<Font> font,
        LITERAL text,
        Vec2i pos,
        RGBA color,
        FLOAT scale
    ) const;

    void Rect(
        Layout layout,
        Vec2i pos,
        Vec2i size,
        RGBA color
    ) const;

    void Border(
        Layout layout,
        Vec2i pos,
        Vec2i size,
        RGBA color
    ) const;

    void Rect(
        Vec2i pos,
        Vec2i size,
        RGBA color
    ) const;

    void Border(
        Vec2i pos,
        Vec2i size,
        RGBA color
    ) const;

    static void Line(
        Vec2i start,
        Vec2i end,
        RGBA color
    );

    void Image(Layout layout, VIEW_PTR<Texture> texture, Vec2i pos, Vec2i size,
        RGBA tint = {.r = 1, .g = 1, .b = 1, .a = 1}, FLOAT scale = 1) const;
};

#endif //FASTENGINE_UICONTEXT_H