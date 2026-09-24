//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_QUAT_H
#define FASTENGINE_QUAT_H

#include <Platform.h>
#include <cmath>

#include "Vector.h"

namespace toolkit {
    class Quat {
    public:
        FLOAT x, y, z, w;
        Quat();
        Quat(FLOAT x, FLOAT y, FLOAT z, FLOAT w);

        static Quat Identity();
        static Quat FromAxisAngle(const Vec3f& axis, FLOAT angleRadians);
        static Quat FromEuler(FLOAT pitch, FLOAT yaw, FLOAT roll);

        void ToMatrix4x4(FLOAT *out) const;

        static Quat FromEuler(const Vec3f& eulerRadians);
        static Quat Slerp(const Quat& a, const Quat& b, FLOAT t);
        static Quat Lerp(const Quat& a, const Quat& b, FLOAT t);

        FLOAT X() const;
        FLOAT Y() const;
        FLOAT Z() const;
        FLOAT W() const;

        FLOAT GetAngle() const {
            const FLOAT clampedW = std::max(-1.0f, std::min(1.0f, w));
            return 2.0f * std::acos(clampedW);
        }

        FLOAT LengthSquared() const;
        FLOAT Length() const;
        Quat Normalized() const;
        void Normalize();

        Quat Conjugate() const;
        Quat Inverse() const;

        FLOAT Dot(const Quat& other) const;

        Vec3f Rotate(const Vec3f& v) const;
        Vec3f ToEuler() const;

        Quat operator*(const Quat& other) const;
        Quat& operator*=(const Quat& other);
        Vec3f operator*(const Vec3f& v) const;

        Quat operator+(const Quat& other) const;
        Quat operator-(const Quat& other) const;
        Quat operator*(FLOAT scalar) const;

        bool operator==(const Quat& other) const;
        bool operator!=(const Quat& other) const;
    };

    INLINE Quat::Quat() : x(0), y(0), z(0), w(1) {}

    INLINE Quat::Quat(FLOAT x, FLOAT y, FLOAT z, FLOAT w) : x(x), y(y), z(z), w(w) {}

    INLINE Quat Quat::Identity() {
        return Quat(0, 0, 0, 1);
    }

    INLINE Quat Quat::FromAxisAngle(const Vec3f& axis, FLOAT angleRadians) {
        const FLOAT halfAngle = angleRadians * 0.5f;
        const FLOAT s = std::sin(halfAngle);
        const FLOAT axisLen = axis.Length();
        if (axisLen < 1e-8f)
            return Identity();
        const Vec3f n = axis / axisLen;
        return Quat(n.x * s, n.y * s, n.z * s, std::cos(halfAngle));
    }

    INLINE Quat Quat::FromEuler(FLOAT pitch, FLOAT yaw, FLOAT roll) {
        const FLOAT cp = std::cos(pitch * 0.5f);
        const FLOAT sp = std::sin(pitch * 0.5f);
        const FLOAT cy = std::cos(yaw * 0.5f);
        const FLOAT sy = std::sin(yaw * 0.5f);
        const FLOAT cr = std::cos(roll * 0.5f);
        const FLOAT sr = std::sin(roll * 0.5f);

        return Quat(
            sp * cy * cr - cp * sy * sr,
            cp * sy * cr + sp * cy * sr,
            cp * cy * sr - sp * sy * cr,
            cp * cy * cr + sp * sy * sr
        );
    }

    INLINE void Quat::ToMatrix4x4(FLOAT* out) const {
        const FLOAT xx = x * x;
        const FLOAT yy = y * y;
        const FLOAT zz = z * z;
        const FLOAT xy = x * y;
        const FLOAT xz = x * z;
        const FLOAT yz = y * z;
        const FLOAT wx = w * x;
        const FLOAT wy = w * y;
        const FLOAT wz = w * z;

        out[0] = 1.0f - 2.0f * (yy + zz);
        out[1] = 2.0f * (xy + wz);
        out[2] = 2.0f * (xz - wy);
        out[3] = 0.0f;

        out[4] = 2.0f * (xy - wz);
        out[5] = 1.0f - 2.0f * (xx + zz);
        out[6] = 2.0f * (yz + wx);
        out[7] = 0.0f;

        out[8] = 2.0f * (xz + wy);
        out[9] = 2.0f * (yz - wx);
        out[10] = 1.0f - 2.0f * (xx + yy);
        out[11] = 0.0f;

        out[12] = 0.0f;
        out[13] = 0.0f;
        out[14] = 0.0f;
        out[15] = 1.0f;
    }

    INLINE Quat Quat::FromEuler(const Vec3f& eulerRadians) {
        return FromEuler(eulerRadians.x, eulerRadians.y, eulerRadians.z);
    }

    INLINE FLOAT Quat::X() const { return x; }
    INLINE FLOAT Quat::Y() const { return y; }
    INLINE FLOAT Quat::Z() const { return z; }
    INLINE FLOAT Quat::W() const { return w; }

    INLINE FLOAT Quat::LengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    INLINE FLOAT Quat::Length() const {
        return std::sqrt(LengthSquared());
    }

    INLINE Quat Quat::Normalized() const {
        const FLOAT len = Length();
        if (len < 1e-8f)
            return Identity();
        const FLOAT inv = 1.0f / len;
        return Quat(x * inv, y * inv, z * inv, w * inv);
    }

    INLINE VOID Quat::Normalize() {
        *this = Normalized();
    }

    INLINE Quat Quat::Conjugate() const {
        return Quat(-x, -y, -z, w);
    }

    INLINE Quat Quat::Inverse() const {
        const FLOAT lenSq = LengthSquared();
        if (lenSq < 1e-8f)
            return Identity();
        const FLOAT invLenSq = 1.0f / lenSq;
        return Quat(-x * invLenSq, -y * invLenSq, -z * invLenSq, w * invLenSq);
    }

    INLINE FLOAT Quat::Dot(const Quat& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    INLINE Quat Quat::operator*(const Quat& other) const {
        return Quat(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    INLINE Quat& Quat::operator*=(const Quat& other) {
        *this = *this * other;
        return *this;
    }

    INLINE Vec3f Quat::Rotate(const Vec3f& v) const {
        const Vec3f u(x, y, z);
        const FLOAT s = w;

        const Vec3f uv = u.Cross(v);
        const Vec3f uuv = u.Cross(uv);

        return v + (uv * (2.0f * s)) + (uuv * 2.0f);
    }

    INLINE Vec3f Quat::operator*(const Vec3f& v) const {
        return Rotate(v);
    }

    INLINE Vec3f Quat::ToEuler() const {
        FLOAT pitch, yaw, roll;

        const FLOAT sinp = 2.0f * (w * x + y * z);
        const FLOAT cosp = 1.0f - 2.0f * (x * x + y * y);
        pitch = std::atan2(sinp, cosp);

        const FLOAT siny = 2.0f * (w * y - z * x);
        if (std::abs(siny) >= 1.0f)
            yaw = std::copysign(3.14159265358979323846f / 2.0f, siny);
        else
            yaw = std::asin(siny);

        const FLOAT sinr = 2.0f * (w * z + x * y);
        const FLOAT cosr = 1.0f - 2.0f * (y * y + z * z);
        roll = std::atan2(sinr, cosr);

        return Vec3f(pitch, yaw, roll);
    }

    INLINE Quat Quat::operator+(const Quat& other) const {
        return Quat(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    INLINE Quat Quat::operator-(const Quat& other) const {
        return Quat(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    INLINE Quat Quat::operator*(FLOAT scalar) const {
        return Quat(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    INLINE bool Quat::operator==(const Quat& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    INLINE bool Quat::operator!=(const Quat& other) const {
        return !(*this == other);
    }

    INLINE Quat Quat::Lerp(const Quat& a, const Quat& b, FLOAT t) {
        return (a * (1.0f - t) + b * t).Normalized();
    }

    INLINE Quat Quat::Slerp(const Quat& a, const Quat& b, FLOAT t) {
        Quat end = b;
        FLOAT cosHalfTheta = a.Dot(b);

        if (cosHalfTheta < 0.0f) {
            end = Quat(-b.x, -b.y, -b.z, -b.w);
            cosHalfTheta = -cosHalfTheta;
        }

        if (cosHalfTheta > 0.9995f)
            return Lerp(a, end, t);

        const FLOAT halfTheta = std::acos(cosHalfTheta);
        const FLOAT sinHalfTheta = std::sqrt(1.0f - cosHalfTheta * cosHalfTheta);

        const FLOAT ratioA = std::sin((1.0f - t) * halfTheta) / sinHalfTheta;
        const FLOAT ratioB = std::sin(t * halfTheta) / sinHalfTheta;

        return (a * ratioA + end * ratioB).Normalized();
    }
}
#endif //FASTENGINE_QUAT_H