//
// Created by dlllibstdntc on 12.09.2026.
//

#include "UIContext.h"

#include <GL/gl.h>

#include "BufferBuilder.h"

UIContext::UIContext(VIEW_PTR<RenderSystem> render_system) : render_system(render_system) {

}

void UIContext::DrawCircle(const Vec2f &pos, const Vec2f &size, const Brush &color, bool fill) {
        constexpr INT SEGMENTS = 32;

        BufferBuilder bb(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP);

        if (fill) {
                const Color center = color.At(0);
                bb.Vertex(pos.x, pos.y, 0.0f,
                          center.GetRed(), center.GetGreen(), center.GetBlue(), center.GetAlpha());
        }

        for (INT i = 0; i <= SEGMENTS; ++i) {
                const FLOAT angle = (2.0f * 3.14159265f * static_cast<FLOAT>(i)) / static_cast<FLOAT>(SEGMENTS);
                const FLOAT x = pos.x + std::cos(angle) * size.x;
                const FLOAT y = pos.y + std::sin(angle) * size.y;

                const Color c = color.At(i % SEGMENTS);

                bb.Vertex(x, y, 0.0f, c.GetRed(), c.GetGreen(), c.GetBlue(), c.GetAlpha());
        }

        bb.Flush();
}


void UIContext::DrawBorder(const Vec2f &pos, const Vec2f &size, const Brush &color) {

        BufferBuilder bb(GL_QUADS);

        const Color topLeft = color.At(0);
        const Color topRight = color.At(1);
        const Color bottomRight = color.At(2);
        const Color bottomLeft = color.At(3);

        bb.Vertex(pos.x, pos.y, 0.0f,
                  topLeft.GetRed(), topLeft.GetGreen(), topLeft.GetBlue(), topLeft.GetAlpha())
          .Vertex(pos.x + size.x, pos.y, 0.0f,
                  topRight.GetRed(), topRight.GetGreen(), topRight.GetBlue(), topRight.GetAlpha())
          .Vertex(pos.x + size.x, pos.y + size.y, 0.0f,
                  bottomRight.GetRed(), bottomRight.GetGreen(), bottomRight.GetBlue(), bottomRight.GetAlpha())
          .Vertex(pos.x, pos.y + size.y, 0.0f,
                  bottomLeft.GetRed(), bottomLeft.GetGreen(), bottomLeft.GetBlue(), bottomLeft.GetAlpha())
          .Flush();
}

void UIContext::DrawRect(const Vec2f &pos, const Vec2f &size, const Brush &color) {
    BufferBuilder bb(GL_LINE_LOOP);

    const Color topLeft = color.At(0);
    const Color topRight = color.At(1);
    const Color bottomRight = color.At(2);
    const Color bottomLeft = color.At(3);

    bb.Vertex(pos.x, pos.y, 0.0f,
              topLeft.GetRed(), topLeft.GetGreen(), topLeft.GetBlue(), topLeft.GetAlpha())
      .Vertex(pos.x + size.x, pos.y, 0.0f,
              topRight.GetRed(), topRight.GetGreen(), topRight.GetBlue(), topRight.GetAlpha())
      .Vertex(pos.x + size.x, pos.y + size.y, 0.0f,
              bottomRight.GetRed(), bottomRight.GetGreen(), bottomRight.GetBlue(), bottomRight.GetAlpha())
      .Vertex(pos.x, pos.y + size.y, 0.0f,
              bottomLeft.GetRed(), bottomLeft.GetGreen(), bottomLeft.GetBlue(), bottomLeft.GetAlpha())
      .Flush();
}

void UIContext::DrawLine(FLOAT y, FLOAT minX, FLOAT maxX, const Brush &color) {
    BufferBuilder bb(GL_LINES);

    const Color start = color.At(0);
    const Color end = color.At(1);

    bb.Vertex(minX, y, 0.0f, start.GetRed(), start.GetGreen(), start.GetBlue(), start.GetAlpha())
      .Vertex(maxX, y, 0.0f, end.GetRed(), end.GetGreen(), end.GetBlue(), end.GetAlpha())
      .Flush();
}
