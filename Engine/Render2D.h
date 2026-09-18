//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_RENDER2D_H
#define FASTENGINE_RENDER2D_H

#include "../Toolkit/Vector.h"
#include "../Toolkit/Color.h"
#include <stb_truetype.h>

struct Box2D;

struct Texture {
    UIHANDLE id;
    INT width, height;
    INT channels;
};

struct Font {
    stbtt_bakedchar cdata[96];
    UIHANDLE fontTexture;
    const int atlasW = 512;
    const int atlasH = 512;

    static GLOBAL_PTR<Font> Get(LITERAL path, INT size);
};

struct Recti {
    INT x, y, width, height;

    static Recti CalculateStepRect(INT atlasWidth, INT atlasHeight, INT frameSize, UINT step);
};

struct TextMetrics {
    Vec2f size;
    float baselineOffset;
};

namespace Render2D {

    Err<GLOBAL_PTR<Texture>> GetTexture(LITERAL path);
    Err<GLOBAL_PTR<Font>> GetFont(LITERAL path, INT size);
    void RenderText(VIEW_PTR<Font> font, const char *text, float px, float py, float r, float g, float b,
        float scale);
    TextMetrics MeasureText(VIEW_PTR<Font> font, const char *text, float scale);

    void DrawCircle(const Vec2f &pos, const Vec2f &size, const Brush& color,
        bool fill = true);
    void DrawTexture(VIEW_PTR<Texture> texture, const Vec2f &pos, const Vec2f &size, const Brush &color, bool flipX = false);
    void DrawTexture(VIEW_PTR<Texture> texture, const Recti &srcRect, const Vec2f &pos, const Vec2f &size, const Brush &color, bool flipX = false);
    void DrawBorder(const Vec2f &pos, const Vec2f &size, const Brush& color);
    void DrawBorder(const Box2D &box, const Brush& color);
    void DrawRect(const Vec2f &pos, const Vec2f &size, const Brush& color);
    void DrawLine(FLOAT y, FLOAT minX, FLOAT maxX, const Brush &color);
    void DrawTextureUI(const Texture& texture, Vec2f pos, Vec2f size, RGBA tint, FLOAT scale = 1);
};


#endif //FASTENGINE_RENDER2D_H
