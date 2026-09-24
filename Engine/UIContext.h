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

    toolkit::Vec2i GetLayoutPosition(
        Layout layout,
        toolkit::Vec2i pos,
        toolkit::Vec2i size
    ) const;

public:
    explicit UIContext(VIEW_PTR<RenderSystem> render_system)
        : render_system(render_system) {
    }

    toolkit::Vec2i GetWindowSize() const;

    void Text(
        Layout layout,
        VIEW_PTR<Font> font,
        LITERAL text,
        toolkit::Vec2i pos,
        toolkit::RGBA color,
        FLOAT scale
    ) const;

    void Rect(
        Layout layout,
        toolkit::Vec2i pos,
        toolkit::Vec2i size,
        toolkit::RGBA color
    ) const;

    void Border(
        Layout layout,
        toolkit::Vec2i pos,
        toolkit::Vec2i size,
        toolkit::RGBA color
    ) const;

    void Rect(
        toolkit::Vec2i pos,
        toolkit::Vec2i size,
        toolkit::RGBA color
    ) const;

    void Border(
        toolkit::Vec2i pos,
        toolkit::Vec2i size,
        toolkit::RGBA color
    ) const;

    static void Line(
        toolkit::Vec2i start,
        toolkit::Vec2i end,
        toolkit::RGBA color
    );

    void Image(Layout layout, VIEW_PTR<Texture> texture, toolkit::Vec2i pos, toolkit::Vec2i size,
        toolkit::RGBA tint = {.r = 1, .g = 1, .b = 1, .a = 1}, FLOAT scale = 1) const;
};

#endif //FASTENGINE_UICONTEXT_H