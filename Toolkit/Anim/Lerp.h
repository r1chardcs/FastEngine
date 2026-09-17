#ifndef FASTENGINE_LERP_H
#define FASTENGINE_LERP_H

#include <Toolkit/Quat.h>
#include <Toolkit/Vector.h>
#include <Toolkit/Color.h>

template <typename T>
struct LerpTraits {
    static T Interpolate(const T& a, const T& b, float t) {
        return a + (b - a) * t;
    }
};

template <>
struct LerpTraits<Vec2f> {
    static Vec2f Interpolate(const Vec2f& a, const Vec2f& b, float t) {
        return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
    }
};

template <>
struct LerpTraits<Vec3f> {
    static Vec3f Interpolate(const Vec3f& a, const Vec3f& b, float t) {
        return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t };
    }
};

template <>
struct LerpTraits<RGBA> {
    static RGBA Interpolate(const RGBA& a, const RGBA& b, float t) {
        return {
            a.r + (b.r - a.r) * t,
            a.g + (b.g - a.g) * t,
            a.b + (b.b - a.b) * t,
            a.a + (b.a - a.a) * t
        };
    }
};

template <>
struct LerpTraits<Quat> {
    static Quat Interpolate(const Quat& a, const Quat& b, float t) {
        float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        Quat target = b;

        if (dot < 0.0f) {
            target = Quat(-b.x, -b.y, -b.z, -b.w);
        }

        Quat result(
            a.x + (target.x - a.x) * t,
            a.y + (target.y - a.y) * t,
            a.z + (target.z - a.z) * t,
            a.w + (target.w - a.w) * t
        );

        return result.Normalized();
    }
};

template <typename T>
T Lerp(const T& a, const T& b, float t) {
    return LerpTraits<T>::Interpolate(a, b, t);
}

#endif