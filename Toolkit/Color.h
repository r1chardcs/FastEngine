//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_COLOR_H
#define FASTENGINE_COLOR_H

#include <Platform.h>

struct RGBA { FLOAT r, g, b, a; };

class Color {
    RGBA rgba;
public:
    Color(MOVE_PLEASE RGBA rgba);
    Color(FLOAT r = 1, FLOAT g = 1, FLOAT b = 1, FLOAT a = 1);

    FLOAT GetRed() const;
    FLOAT GetBlue() const;
    FLOAT GetGreen() const;
    FLOAT GetAlpha() const;

    RGBA& GetRGBA();
};

class Brush {
    VECTOR<Color> colors;
public:
    template <typename... T>
    Brush(const Color &color, T... args) {
        colors.push_back(color);
        (colors.push_back(args), ...);
    }

    void Put(const Color &color);
    Color At(INT index) const;
};

#endif //FASTENGINE_COLOR_H
