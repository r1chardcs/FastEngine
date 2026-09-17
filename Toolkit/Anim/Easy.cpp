//
// Created by dlllibstdntc on 17.09.2026.
//

#include "Easy.h"

float Ease::Linear(float t) { return t; }

float Ease::QuadIn(float t) { return t * t; }

float Ease::QuadOut(float t) { return t * (2.0f - t); }

float Ease::QuadInOut(float t) {
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

float Ease::CubicIn(float t) { return t * t * t; }

float Ease::CubicOut(float t) {
    const float f = t - 1.0f;
    return f * f * f + 1.0f;
}

float Ease::CubicInOut(float t) {
    return t < 0.5f
               ? 4.0f * t * t * t
               : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
}

float Ease::SineIn(float t) { return 1.0f - std::cos(t * 1.57079632679f); }

float Ease::SineOut(float t) { return std::sin(t * 1.57079632679f); }

float Ease::SineInOut(float t) { return -0.5f * (std::cos(3.14159265359f * t) - 1.0f); }

float Ease::ExpoIn(float t) { return t <= 0.0f ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f)); }

float Ease::ExpoOut(float t) { return t >= 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t); }

float Ease::BackOut(float t) {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    const float f = t - 1.0f;
    return 1.0f + c3 * f * f * f + c1 * f * f;
}

float Ease::ElasticOut(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float c4 = 2.0f * 3.14159265359f / 3.0f;
    return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float Ease::BounceOut(float t) {
    const float n1 = 7.5625f;
    const float d1 = 2.75f;

    if (t < 1.0f / d1) {
        return n1 * t * t;
    }
    if (t < 2.0f / d1) {
        t -= 1.5f / d1;
        return n1 * t * t + 0.75f;
    }
    if (t < 2.5f / d1) {
        t -= 2.25f / d1;
        return n1 * t * t + 0.9375f;
    }
    t -= 2.625f / d1;
    return n1 * t * t + 0.984375f;
}
