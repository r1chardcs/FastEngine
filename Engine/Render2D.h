//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_RENDER2D_H
#define FASTENGINE_RENDER2D_H

#include "../Toolkit/Vector.h"
#include "../Toolkit/Color.h"

struct Box2D;

struct Texture {
    UIHANDLE id;
    INT width, height;
    INT channels;
};

struct Recti {
    INT x, y, width, height;
};

namespace Render2D {
    Err<Texture> GetTexture(LITERAL path);
    void DrawCircle(const Vec2f &pos, const Vec2f &size, const Brush& color,
        bool fill = true);
    void DrawTexture(const Texture &texture, const Vec2f &pos, const Vec2f &size, const Brush &color);
    void DrawTexture(const Texture &texture, const Recti &srcRect, const Vec2f &pos, const Vec2f &size, const Brush &color);
    void DrawBorder(const Vec2f &pos, const Vec2f &size, const Brush& color);
    void DrawBorder(const Box2D &box, const Brush& color);
    void DrawRect(const Vec2f &pos, const Vec2f &size, const Brush& color);
    void DrawLine(FLOAT y, FLOAT minX, FLOAT maxX, const Brush &color);
};


#endif //FASTENGINE_RENDER2D_H
