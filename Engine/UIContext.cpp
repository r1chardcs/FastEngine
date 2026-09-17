//
// Created by dlllibstdntc on 12.09.2026.
//

#include "UIContext.h"

#include <GL/gl.h>

#include "App.h"
#include "BufferBuilder.h"

Vec2i UIContext::GetWindowSize() const {
    const auto window = render_system->GetApp()->GetWindow();

    return {
        window->GetWidth(),
        window->GetHeight()
    };
}

Vec2i UIContext::GetLayoutPosition(
    Layout layout,
    Vec2i pos,
    Vec2i size
) const {
    const Vec2i window = GetWindowSize();

    switch (layout) {
        case Layout::UP_LEFT:
            return {
                pos.x,
                pos.y
            };

        case Layout::UP_CENTER:
            return {
                window.x / 2 - size.x / 2 + pos.x,
                pos.y
            };

        case Layout::UP_RIGHT:
            return {
                window.x - size.x - pos.x,
                pos.y
            };

        case Layout::CENTER_LEFT:
            return {
                pos.x,
                window.y / 2 - size.y / 2 + pos.y
            };

        case Layout::CENTER:
            return {
                window.x / 2 - size.x / 2 + pos.x,
                window.y / 2 - size.y / 2 + pos.y
            };

        case Layout::CENTER_RIGHT:
            return {
                window.x - size.x - pos.x,
                window.y / 2 - size.y / 2 + pos.y
            };

        case Layout::DOWN_LEFT:
            return {
                pos.x,
                window.y - size.y - pos.y
            };

        case Layout::DOWN_CENTER:
            return {
                window.x / 2 - size.x / 2 + pos.x,
                window.y - size.y - pos.y
            };

        case Layout::DOWN_RIGHT:
            return {
                window.x - size.x - pos.x,
                window.y - size.y - pos.y
            };
    }

    return pos;
}

void UIContext::Text(
    Layout layout,
    const Font& font,
    const char* text,
    Vec2i pos,
    RGBA color,
    FLOAT scale
) {
    if (!render_system || !text) {
        return;
    }

    const auto metrics = Render2D::MeasureText(font, text, scale);
    const Vec2i size = {
        static_cast<INT>(metrics.size.x),
        static_cast<INT>(metrics.size.y)
    };

    Vec2i finalPos = GetLayoutPosition(layout, pos, size);
    finalPos.y += static_cast<INT>(metrics.baselineOffset);

    Render2D::RenderText(
        font,
        text,
        static_cast<FLOAT>(finalPos.x),
        static_cast<FLOAT>(finalPos.y),
        color.r,
        color.g,
        color.b,
        scale
    );
}

void UIContext::Rect(
    Layout layout,
    Vec2i pos,
    Vec2i size,
    RGBA color
) {
    const Vec2i finalPos = GetLayoutPosition(layout, pos, size);

    Render2D::DrawRect(
        {
            static_cast<FLOAT>(finalPos.x),
            static_cast<FLOAT>(finalPos.y)
        },
        {
            static_cast<FLOAT>(size.x),
            static_cast<FLOAT>(size.y)
        },
        Brush(color)
    );
}

void UIContext::Border(
    Layout layout,
    Vec2i pos,
    Vec2i size,
    RGBA color
) {
    const Vec2i finalPos = GetLayoutPosition(layout, pos, size);

    Render2D::DrawBorder(
        {
            static_cast<FLOAT>(finalPos.x),
            static_cast<FLOAT>(finalPos.y)
        },
        {
            static_cast<FLOAT>(size.x),
            static_cast<FLOAT>(size.y)
        },
        Brush(color)
    );
}

void UIContext::Rect(
    Vec2i pos,
    Vec2i size,
    RGBA color
) {
    Rect(
        Layout::UP_LEFT,
        pos,
        size,
        color
    );
}

void UIContext::Border(
    Vec2i pos,
    Vec2i size,
    RGBA color
) {
    Border(
        Layout::UP_LEFT,
        pos,
        size,
        color
    );
}

void UIContext::Line(
    Vec2i start,
    Vec2i end,
    RGBA color
) {
    BufferBuilder bb(GL_LINES);


    bb.Vertex(
        static_cast<FLOAT>(start.x),
        static_cast<FLOAT>(start.y),
        0.0f,
        color.r, color.g, color.b, color.a
    );

    bb.Vertex(
        static_cast<FLOAT>(end.x),
        static_cast<FLOAT>(end.y),
        0.0f,
        color.r, color.g, color.b, color.a
    );

    bb.Flush();
}