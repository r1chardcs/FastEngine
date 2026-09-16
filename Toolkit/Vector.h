//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_VECTOR_H
#define FASTENGINE_VECTOR_H

#include <Platform.h>
#include <cmath>

template <typename NumberType>
struct Vec2 {
    NumberType x;
    NumberType y;

    Vec2() : x(NumberType(0)), y(NumberType(0)) {}
    Vec2(NumberType x, NumberType y) : x(x), y(y) {}

    static Vec2 Zero() { return Vec2(NumberType(0), NumberType(0)); }
    static Vec2 One() { return Vec2(NumberType(1), NumberType(1)); }
    static Vec2 UnitX() { return Vec2(NumberType(1), NumberType(0)); }
    static Vec2 UnitY() { return Vec2(NumberType(0), NumberType(1)); }

    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(const Vec2& other) const { return Vec2(x * other.x, y * other.y); }
    Vec2 operator/(const Vec2& other) const { return Vec2(x / other.x, y / other.y); }

    Vec2 operator*(NumberType scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator/(NumberType scalar) const { return Vec2(x / scalar, y / scalar); }

    Vec2 operator-() const { return Vec2(-x, -y); }

    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    Vec2& operator*=(const Vec2& other) { x *= other.x; y *= other.y; return *this; }
    Vec2& operator/=(const Vec2& other) { x /= other.x; y /= other.y; return *this; }

    Vec2& operator*=(NumberType scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2& operator/=(NumberType scalar) { x /= scalar; y /= scalar; return *this; }

    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vec2& other) const { return !(*this == other); }

    NumberType& operator[](int index) { return index == 0 ? x : y; }
    const NumberType& operator[](int index) const { return index == 0 ? x : y; }

    NumberType Dot(const Vec2& other) const { return x * other.x + y * other.y; }
    NumberType Cross(const Vec2& other) const { return x * other.y - y * other.x; }

    NumberType LengthSquared() const { return x * x + y * y; }
    NumberType Length() const { return static_cast<NumberType>(std::sqrt(static_cast<double>(LengthSquared()))); }

    Vec2 Normalized() const {
        NumberType len = Length();
        if (len == NumberType(0)) return Vec2::Zero();
        return Vec2(x / len, y / len);
    }

    void Normalize() {
        NumberType len = Length();
        if (len == NumberType(0)) return;
        x /= len;
        y /= len;
    }

    NumberType DistanceTo(const Vec2& other) const { return (*this - other).Length(); }
    NumberType DistanceSquaredTo(const Vec2& other) const { return (*this - other).LengthSquared(); }

    static Vec2 Lerp(const Vec2& a, const Vec2& b, NumberType t) {
        return a + (b - a) * t;
    }

    template <typename Other>
    Vec2<Other> Cast() const {
        return Vec2<Other>(static_cast<Other>(x), static_cast<Other>(y));
    }
};

template <typename NumberType>
Vec2<NumberType> operator*(NumberType scalar, const Vec2<NumberType>& v) {
    return v * scalar;
}

template <typename NumberType>
struct Vec3 {
    NumberType x;
    NumberType y;
    NumberType z;

    Vec3() : x(NumberType(0)), y(NumberType(0)), z(NumberType(0)) {}
    Vec3(NumberType x, NumberType y, NumberType z) : x(x), y(y), z(z) {}
    Vec3(const Vec2<NumberType>& xy, NumberType z) : x(xy.x), y(xy.y), z(z) {}

    static Vec3 Zero() { return Vec3(NumberType(0), NumberType(0), NumberType(0)); }
    static Vec3 One() { return Vec3(NumberType(1), NumberType(1), NumberType(1)); }
    static Vec3 UnitX() { return Vec3(NumberType(1), NumberType(0), NumberType(0)); }
    static Vec3 UnitY() { return Vec3(NumberType(0), NumberType(1), NumberType(0)); }
    static Vec3 UnitZ() { return Vec3(NumberType(0), NumberType(0), NumberType(1)); }

    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(const Vec3& other) const { return Vec3(x * other.x, y * other.y, z * other.z); }
    Vec3 operator/(const Vec3& other) const { return Vec3(x / other.x, y / other.y, z / other.z); }

    Vec3 operator*(NumberType scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
    Vec3 operator/(NumberType scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }

    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vec3& operator*=(const Vec3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
    Vec3& operator/=(const Vec3& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }

    Vec3& operator*=(NumberType scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vec3& operator/=(NumberType scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    bool operator==(const Vec3& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(const Vec3& other) const { return !(*this == other); }

    NumberType& operator[](int index) { return index == 0 ? x : (index == 1 ? y : z); }
    const NumberType& operator[](int index) const { return index == 0 ? x : (index == 1 ? y : z); }

    NumberType Dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }

    Vec3 Cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    NumberType LengthSquared() const { return x * x + y * y + z * z; }
    NumberType Length() const { return static_cast<NumberType>(std::sqrt(static_cast<double>(LengthSquared()))); }

    Vec3 Normalized() const {
        NumberType len = Length();
        if (len == NumberType(0)) return Vec3::Zero();
        return Vec3(x / len, y / len, z / len);
    }

    void Normalize() {
        NumberType len = Length();
        if (len == NumberType(0)) return;
        x /= len;
        y /= len;
        z /= len;
    }

    NumberType DistanceTo(const Vec3& other) const { return (*this - other).Length(); }
    NumberType DistanceSquaredTo(const Vec3& other) const { return (*this - other).LengthSquared(); }

    static Vec3 Lerp(const Vec3& a, const Vec3& b, NumberType t) {
        return a + (b - a) * t;
    }

    Vec2<NumberType> XY() const { return Vec2<NumberType>(x, y); }

    template <typename Other>
    Vec3<Other> Cast() const {
        return Vec3<Other>(static_cast<Other>(x), static_cast<Other>(y), static_cast<Other>(z));
    }

    Vec2<NumberType> ToVec2() {
        return {x, y};
    }
};

template <typename NumberType>
Vec3<NumberType> operator*(NumberType scalar, const Vec3<NumberType>& v) {
    return v * scalar;
}

using Vec2f = Vec2<FLOAT>;
using Vec2d = Vec2<DOUBLE>;
using Vec2i = Vec2<INT>;

using Vec3f = Vec3<FLOAT>;
using Vec3d = Vec3<DOUBLE>;
using Vec3i = Vec3<INT>;


#endif //FASTENGINE_VECTOR_H
