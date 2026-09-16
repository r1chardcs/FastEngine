//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_RENDER2D_H
#define FASTENGINE_RENDER2D_H

#include "../Toolkit/Vector.h"
#include "../Toolkit/Color.h"

struct Texture {
    UIHANDLE id;
    INT width, height;
    INT channels;
};

namespace Render2D {
    Err<Texture> GetTexture(LITERAL path);
    void DrawTexture(const Texture& texture, const Vec2f& pos, const Vec2f& size, const Brush& color);
    void DrawTexture();
    void DrawCircle(const Vec2f &pos, const Vec2f &size, const Brush& color,
        bool fill = true);

    void DrawBorder(const Vec2f &pos, const Vec2f &size, const Brush& color);
    void DrawRect(const Vec2f &pos, const Vec2f &size, const Brush& color);
    void DrawLine(FLOAT y, FLOAT minX, FLOAT maxX, const Brush &color);
};


#endif //FASTENGINE_RENDER2D_H
