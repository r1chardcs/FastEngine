//
// Created by dlllibstdntc on 12.09.2026.
//

#include "Color.h"

#include <stdarg.h>

Color::Color(const RGBA rgba) : rgba(MOVE(rgba)) {}

Color::Color(FLOAT r, FLOAT g, FLOAT b, FLOAT a) : rgba({
        .r = r,.g = g,
        .b = b, .a = a
    }) {

}

FLOAT Color::GetRed() const {
    return rgba.r;
}

FLOAT Color::GetBlue() const {
    return rgba.b;
}

FLOAT Color::GetGreen() const {
    return rgba.g;
}

FLOAT Color::GetAlpha() const {
    return rgba.a;
}

RGBA & Color::GetRGBA() {
    return rgba;
}

void Brush::Put(const Color& color) {
    colors.push_back(color);
}

Color Brush::At(const INT index) const {
    if (index < colors.size()) {
        return colors[index];
    }
    return colors[0];
}

Brush Brush::Solid(BYTE r, BYTE g, BYTE b, BYTE a) {
    return Brush(Color(r, g, b, a));
}

Brush Brush::Solid(const Color &color) {
    return Brush(color);
}


