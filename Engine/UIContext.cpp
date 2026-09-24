//
// Created by dlllibstdntc on 12.09.2026.
//

#include "UIContext.h"

#include <GL/gl.h>

#include "App.h"
#include "BufferBuilder.h"

toolkit::Vec2i UIContext::GetLayoutPosition(
    const Layout layout,
    toolkit::Vec2i pos,
    const toolkit::Vec2i size
) const {
    const toolkit::Vec2i window = GetWindowSize();

    switch (layout) {
        case Layout::UP_LEFT:
            return {pos.x, pos.y};
        case Layout::UP_CENTER:
            return {window.x / 2 - size.x / 2 + pos.x, pos.y};
        case Layout::UP_RIGHT:
            return {window.x - size.x - pos.x, pos.y};
        case Layout::CENTER_LEFT:
            return {pos.x, window.y / 2 - size.y / 2 + pos.y};
        case Layout::CENTER:
            return {window.x / 2 - size.x / 2 + pos.x, window.y / 2 - size.y / 2 + pos.y};
        case Layout::CENTER_RIGHT:
            return {window.x - size.x - pos.x, window.y / 2 - size.y / 2 + pos.y};
        case Layout::DOWN_LEFT:
            return {pos.x, window.y - size.y - pos.y};
        case Layout::DOWN_CENTER:
            return {window.x / 2 - size.x / 2 + pos.x, window.y - size.y - pos.y};
        case Layout::DOWN_RIGHT:
            return {window.x - size.x - pos.x, window.y - size.y - pos.y};
    }

    return pos;
}

toolkit::Vec2i UIContext::GetWindowSize() const {
    const auto window = render_system->GetApp()->GetWindow();

    return {
        window->GetWidth(),
        window->GetHeight()
    };
}

void UIContext::Text(
    const Layout layout,
    const VIEW_PTR<Font> font,
    const char* text,
    const toolkit::Vec2i pos,
    const toolkit::RGBA color,
    const FLOAT scale
) const {
    if (!render_system || !text) {
        return;
    }

    const auto metrics = Render2D::MeasureText(font, text, scale);
    const toolkit::Vec2i size = {
        static_cast<INT>(metrics.size.x),
        static_cast<INT>(metrics.size.y)
    };

    toolkit::Vec2i finalPos = GetLayoutPosition(layout, pos, size);
    finalPos.y += static_cast<INT>(metrics.baselineOffset);

    Render2D::RenderText(
        font, text,
        static_cast<FLOAT>(finalPos.x), static_cast<FLOAT>(finalPos.y),
        color.r, color.g, color.b,
        scale
    );
}

void UIContext::Rect(
    const Layout layout,
    const toolkit::Vec2i pos,
    const toolkit::Vec2i size,
    const toolkit::RGBA color
) const {
    const toolkit::Vec2i finalPos = GetLayoutPosition(layout, pos, size);

    Render2D::DrawRect(
        {static_cast<FLOAT>(finalPos.x), static_cast<FLOAT>(finalPos.y)},
        {static_cast<FLOAT>(size.x), static_cast<FLOAT>(size.y)}, toolkit::Brush(color));
}

void UIContext::Border(
    const Layout layout,
    const toolkit::Vec2i pos,
    const toolkit::Vec2i size,
    const toolkit::RGBA color
) const {
    const toolkit::Vec2i finalPos = GetLayoutPosition(layout, pos, size);

    Render2D::DrawBorder(
        {static_cast<FLOAT>(finalPos.x), static_cast<FLOAT>(finalPos.y)},
        {static_cast<FLOAT>(size.x), static_cast<FLOAT>(size.y)},
            toolkit::Brush(color)
    );
}

void UIContext::Rect(
    const toolkit::Vec2i pos,
    const toolkit::Vec2i size,
    const toolkit::RGBA color
) const {
    Rect(Layout::UP_LEFT, pos, size, color);
}

void UIContext::Border(
    const toolkit::Vec2i pos,
    const toolkit::Vec2i size,
    const toolkit::RGBA color
) const {
    Border(Layout::UP_LEFT, pos, size, color);
}

void UIContext::Line(
    const toolkit::Vec2i start,
    const toolkit::Vec2i end,
    const toolkit::RGBA color
) {
    BufferBuilder bb(GL_LINES);

    bb.Vertex(
        static_cast<FLOAT>(start.x), static_cast<FLOAT>(start.y), 0.0f,
        color.r, color.g, color.b, color.a);

    bb.Vertex(
        static_cast<FLOAT>(end.x), static_cast<FLOAT>(end.y), 0.0f,
        color.r, color.g, color.b, color.a
    );

    bb.Flush();
}

void UIContext::Image(
    const Layout layout,
    const VIEW_PTR<Texture> texture,
    const toolkit::Vec2i pos,
    const toolkit::Vec2i size,
    const toolkit::RGBA tint,
    const FLOAT scale
) const {
    const toolkit::Vec2i scaledSize = {
        static_cast<INT>(static_cast<FLOAT>(size.x) * scale),
        static_cast<INT>(static_cast<FLOAT>(size.y) * scale)
    };

    const toolkit::Vec2i finalPos = GetLayoutPosition(layout, pos, scaledSize);
    const toolkit::Vec2f center = {
        static_cast<FLOAT>(finalPos.x) + static_cast<FLOAT>(scaledSize.x) / 2.0f,
        static_cast<FLOAT>(finalPos.y) + static_cast<FLOAT>(scaledSize.y) / 2.0f
    };

    Render2D::DrawTextureUI(
        *texture, center, {
            static_cast<FLOAT>(size.x),
            static_cast<FLOAT>(size.y)},
        tint,
        scale
    );
}
