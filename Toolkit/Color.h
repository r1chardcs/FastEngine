#ifndef FASTENGINE_COLOR_H
#define FASTENGINE_COLOR_H

#include <Platform.h>
#include <array>

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
    static constexpr INT kMaxColors = 4;

    std::array<Color, kMaxColors> colors{Color(), Color(), Color(), Color()};
    INT count = 0;

public:
    Brush(const RGBA &rgba) {
        colors[0] = Color(rgba);
        count = 1;
    }

    template <typename... T>
    Brush(const Color &color, T... args) {
        colors[0] = color;
        count = 1;
        (Put(args), ...);
    }

    void Put(const Color &color);

    Color At(INT index) const;

    RGBA GetRGBA();

    static Brush Solid(BYTE r, BYTE g, BYTE b, BYTE a = 255);
    static Brush Solid(const Color &color);
};

#endif //FASTENGINE_COLOR_H