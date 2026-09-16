//
// Created by dlllibstdntc on 12.09.2026.
//

#include "Render2D.h"

#include <GL/gl.h>

#include "BufferBuilder.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Toolkit/Debug/Logger.h"

Err<Texture> Render2D::GetTexture(LITERAL path)
{
    Texture texture = {};
    auto bytes = stbi_load(path, &texture.width, &texture.height, &texture.channels, STBI_rgb_alpha);
    if (!bytes) {
        return {.res = texture, .err = "Error load texture"};
    }

    glGenTextures(1, &texture.id);
    if (texture.id == 0) {
        return {
            .res = texture, .err = "Error generate texture"
        };
    }

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    return {
        .res = texture,
        .err = nullptr
    };
}

void Render2D::DrawTexture(const Texture &texture, const Vec2f &pos, const Vec2f &size, const Brush &color) {
    if (texture.id == 0) {
        LOGERR.Output("Invalid Draw texture in pos %f %f\n", pos.x, pos.y);
        return;
    }


}

void Render2D::DrawCircle(const Vec2f &pos, const Vec2f &size, const Brush &color, bool fill) {
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

void Render2D::DrawBorder(const Vec2f &pos, const Vec2f &size, const Brush &color) {
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

void Render2D::DrawRect(const Vec2f &pos, const Vec2f &size, const Brush &color) {
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

void Render2D::DrawLine(FLOAT y, FLOAT minX, FLOAT maxX, const Brush &color) {
    BufferBuilder bb(GL_LINES);

    const Color start = color.At(0);
    const Color end = color.At(1);

    bb.Vertex(minX, y, 0.0f, start.GetRed(), start.GetGreen(), start.GetBlue(), start.GetAlpha())
            .Vertex(maxX, y, 0.0f, end.GetRed(), end.GetGreen(), end.GetBlue(), end.GetAlpha())
            .Flush();
}
