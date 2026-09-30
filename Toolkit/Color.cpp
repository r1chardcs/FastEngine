//
// Created by dlllibstdntc on 12.09.2026.
//

#include "Color.h"

#include <stdarg.h>

#include "Debug/Test.h"

namespace toolkit {
    Color::Color(const RGBA rgba) : rgba(MOVE(rgba)) {}

    Color::Color(const FLOAT r,
                 const FLOAT g,
                 const FLOAT b, const FLOAT a):
        rgba({.r = r,.g = g, .b = b, .a = a}) { }

    FLOAT Color::GetRed() const {
        TEST_THIS(Color);
        return rgba.r;
    }

    FLOAT Color::GetBlue() const {
        TEST_THIS(Color);
        return rgba.b;
    }

    FLOAT Color::GetGreen() const {
        TEST_THIS(Color);
        return rgba.g;
    }

    FLOAT Color::GetAlpha() const {
        TEST_THIS(Color);
        return rgba.a;
    }

    RGBA & Color::GetRGBA() {
        TEST_THIS(Color);
        return rgba;
    }

    Brush::Brush(const RGBA &rgba) {
        colors[0] = Color(rgba);
        count = 1;
    }

    void Brush::Put(const Color& color) {
        TEST_THIS(Color);
        if (count < kMaxColors) {
            colors[count++] = color;
        }
    }

    Color Brush::At(const INT index) const {
        TEST_THIS(Color);
        if (index < count) {
            return colors[index];
        }
        return colors[0];
    }

    RGBA Brush::GetRGBA() {
        TEST_THIS(Color);
        return colors[0].GetRGBA();
    }

    Brush Brush::Solid(BYTE r, BYTE g, BYTE b, BYTE a) {
        return Brush(Color(r, g, b, a));
    }

    Brush Brush::Solid(const Color &color) {
        return Brush(color);
    }
}
