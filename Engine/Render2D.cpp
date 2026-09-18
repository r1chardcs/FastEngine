//
// Created by dlllibstdntc on 12.09.2026.
//

#include "Render2D.h"

#include <GL/gl.h>

#include "BufferBuilder.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Toolkit/Debug/Logger.h"
#include "../Toolkit/Box2D.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

Err<Texture> Render2D::GetTexture(LITERAL path)
{
    Texture texture = {};

    stbi_set_flip_vertically_on_load(true);
    auto bytes = stbi_load(path, &texture.width, &texture.height, &texture.channels, STBI_rgb_alpha);
    if (!bytes) {
        return {.res = texture, .err = "Error load texture"};
    }

    glGenTextures(1, &texture.id);
    if (texture.id == 0) {
        stbi_image_free(bytes);
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

    stbi_image_free(bytes);

    return {
        .res = texture,
        .err = nullptr
    };
}

static std::unordered_map<std::string, GLOBAL_PTR<Font>> s_fontCache;

Err<GLOBAL_PTR<Font>> Render2D::GetFont(LITERAL path, INT _size) {
    std::string key = std::string(path) + "#" + std::to_string(_size);

    auto it = s_fontCache.find(key);
    if (it != s_fontCache.end()) {
        return {.res = it->second, .err = nullptr};
    }

    const auto font = MakeGlobalPtr<Font>();

    FILE* f = fopen(path, "rb");
    if (!f) {
        return {.res = nullptr, .err = "failed to open font file"};
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    std::vector<unsigned char> ttfBuffer(size);
    fread(ttfBuffer.data(), 1, size, f);
    fclose(f);

    std::vector<unsigned char> alphaOnly(font->atlasW * font->atlasH);

    stbtt_BakeFontBitmap(ttfBuffer.data(), 0, _size,
                          alphaOnly.data(), font->atlasW, font->atlasH,
                          32, 96, font->cdata);

    std::vector<unsigned char> la(font->atlasW * font->atlasH * 2);
    for (int i = 0; i < font->atlasW * font->atlasH; i++) {
        la[i * 2 + 0] = 255;
        la[i * 2 + 1] = alphaOnly[i];
    }

    glGenTextures(1, &font->fontTexture);
    glBindTexture(GL_TEXTURE_2D, font->fontTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, font->atlasW, font->atlasH, 0,
                 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, la.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    s_fontCache[key] = font;

    return {.res = font, .err = nullptr};
}

TextMetrics Render2D::MeasureText(VIEW_PTR<Font> font, const char *text, float scale) {
    float x = 0, y = 0;
    float minY = 0.0f, maxY = 0.0f;

    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font->cdata, font->atlasW, font->atlasH, *text - 32, &x, &y, &q, 1);
            minY = std::min(minY, q.y0);
            maxY = std::max(maxY, q.y1);
        }
        text++;
    }

    return {
        .size = { x * scale, (maxY - minY) * scale },
        .baselineOffset = -minY * scale
    };
}
void Render2D::RenderText(VIEW_PTR<Font> font, const char *text, float px, float py, float r, float g, float b, float scale) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, font->fontTexture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor4f(r, g, b, 1.0f);

    glPushMatrix();
    glTranslatef(px, py, 0);
    glScalef(scale, scale, 1.0f);

    float x = 0, y = 0;
    glBegin(GL_QUADS);
    while (*text) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font->cdata, font->atlasW, font->atlasH, *text - 32, &x, &y, &q, 1);

            glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
            glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
            glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
            glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
        }
        text++;
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Render2D::DrawTexture(const Texture &texture, const Vec2f &pos, const Vec2f &size, const Brush &color, bool flipX) {
    DrawTexture(texture, Recti{0, 0, texture.width, texture.height}, pos, size, color, flipX);
}

void Render2D::DrawTexture(const Texture &texture, const Recti &srcRect, const Vec2f &pos, const Vec2f &size, const Brush &color, bool flipX) {
    if (texture.id == 0) {
        LOGERR.Output("Invalid Draw texture in pos %f %f\n", pos.x, pos.y);
        return;
    }

    if (texture.width <= 0 || texture.height <= 0) {
        LOGERR.Output("Invalid texture dimensions for atlas region draw\n");
        return;
    }

    const FLOAT texW = static_cast<FLOAT>(texture.width);
    const FLOAT texH = static_cast<FLOAT>(texture.height);

    FLOAT u0 = static_cast<FLOAT>(srcRect.x) / texW;
    FLOAT v0 = static_cast<FLOAT>(srcRect.y) / texH;
    FLOAT u1 = static_cast<FLOAT>(srcRect.x + srcRect.width) / texW;
    FLOAT v1 = static_cast<FLOAT>(srcRect.y + srcRect.height) / texH;
    if (flipX) {
        std::swap(u0, u1);
    }
    const Color topLeft = color.At(0);
    const Color topRight = color.At(1);
    const Color bottomRight = color.At(2);
    const Color bottomLeft = color.At(3);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture.id);

    glBegin(GL_QUADS);

    glColor4ub(topLeft.GetRed(), topLeft.GetGreen(), topLeft.GetBlue(), topLeft.GetAlpha());
    glTexCoord2f(u0, v0);
    glVertex3f(pos.x, pos.y, 0.0f);

    glColor4ub(topRight.GetRed(), topRight.GetGreen(), topRight.GetBlue(), topRight.GetAlpha());
    glTexCoord2f(u1, v0);
    glVertex3f(pos.x + size.x, pos.y, 0.0f);

    glColor4ub(bottomRight.GetRed(), bottomRight.GetGreen(), bottomRight.GetBlue(), bottomRight.GetAlpha());
    glTexCoord2f(u1, v1);
    glVertex3f(pos.x + size.x, pos.y + size.y, 0.0f);

    glColor4ub(bottomLeft.GetRed(), bottomLeft.GetGreen(), bottomLeft.GetBlue(), bottomLeft.GetAlpha());
    glTexCoord2f(u0, v1);
    glVertex3f(pos.x, pos.y + size.y, 0.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
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

void Render2D::DrawBorder(const Box2D &box, const Brush &color) {
    BufferBuilder bb(GL_LINE_LOOP);

    const Color topLeft = color.At(0);
    const Color topRight = color.At(1);
    const Color bottomRight = color.At(2);
    const Color bottomLeft = color.At(3);

    bb.Vertex(box.minX, box.minY, 0.0f,
              topLeft.GetRed(), topLeft.GetGreen(), topLeft.GetBlue(), topLeft.GetAlpha())
            .Vertex(box.maxX, box.minY, 0.0f,
                    topRight.GetRed(), topRight.GetGreen(), topRight.GetBlue(), topRight.GetAlpha())
            .Vertex(box.maxX, box.maxY, 0.0f,
                    bottomRight.GetRed(), bottomRight.GetGreen(), bottomRight.GetBlue(), bottomRight.GetAlpha())
            .Vertex(box.minX, box.maxY, 0.0f,
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

Recti Recti::CalculateStepRect(INT atlasWidth, INT atlasHeight, INT frameSize, UINT step) {
    const INT framesPerRow = frameSize > 0 ? atlasWidth / frameSize : 0;
    const INT framesPerCol = frameSize > 0 ? atlasHeight / frameSize : 0;
    const INT totalFrames = framesPerRow * framesPerCol;

    if (framesPerRow <= 0 || totalFrames <= 0) {
        return Recti{0, 0, atlasWidth, atlasHeight};
    }

    const UINT clampedStep = step % totalFrames;

    const INT col = static_cast<INT>(clampedStep) % framesPerRow;
    const INT row = static_cast<INT>(clampedStep) / framesPerRow;

    return Recti{
        .x = col * frameSize,
        .y = row * frameSize,
        .width = frameSize,
        .height = frameSize
    };
}
