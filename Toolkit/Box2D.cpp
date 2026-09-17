//
// Created by dlllibstdntc on 17.09.2026.
//

#include "Box2D.h"

#include <cmath>

Box2D::Box2D(): minX(0.0f), minY(0.0f), maxX(0.0f), maxY(0.0f) {}

Box2D::Box2D(float minX_, float minY_, float maxX_, float maxY_): minX(minX_), minY(minY_), maxX(maxX_), maxY(maxY_) {}

Box2D Box2D::FromCenterSize(float centerX, float centerY, float width, float height) {
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    return Box2D(centerX - hw, centerY - hh, centerX + hw, centerY + hh);
}

float Box2D::Width() const { return maxX - minX; }

float Box2D::Height() const { return maxY - minY; }

float Box2D::Area() const { return Width() * Height(); }

float Box2D::CenterX() const { return (minX + maxX) * 0.5f; }

float Box2D::CenterY() const { return (minY + maxY) * 0.5f; }

bool Box2D::IsEmpty() const { return minX >= maxX || minY >= maxY; }

bool Box2D::IsValid() const { return minX <= maxX && minY <= maxY; }

bool Box2D::Contains(float x, float y) const {
    return x >= minX && x <= maxX && y >= minY && y <= maxY;
}

bool Box2D::Contains(const Box2D &other) const {
    return other.minX >= minX && other.maxX <= maxX &&
           other.minY >= minY && other.maxY <= maxY;
}

bool Box2D::Intersects(const Box2D &other) const {
    return minX < other.maxX && maxX > other.minX &&
           minY < other.maxY && maxY > other.minY;
}

Box2D Box2D::Intersection(const Box2D &other) const {
    float nx0 = minX > other.minX ? minX : other.minX;
    float ny0 = minY > other.minY ? minY : other.minY;
    float nx1 = maxX < other.maxX ? maxX : other.maxX;
    float ny1 = maxY < other.maxY ? maxY : other.maxY;
    return Box2D(nx0, ny0, nx1, ny1);
}

Box2D Box2D::Union(const Box2D &other) const {
    float nx0 = minX < other.minX ? minX : other.minX;
    float ny0 = minY < other.minY ? minY : other.minY;
    float nx1 = maxX > other.maxX ? maxX : other.maxX;
    float ny1 = maxY > other.maxY ? maxY : other.maxY;
    return Box2D(nx0, ny0, nx1, ny1);
}

void Box2D::Expand(float amount) {
    minX -= amount;
    minY -= amount;
    maxX += amount;
    maxY += amount;
}

void Box2D::Encapsulate(float x, float y) {
    if (x < minX) minX = x;
    if (y < minY) minY = y;
    if (x > maxX) maxX = x;
    if (y > maxY) maxY = y;
}

void Box2D::Encapsulate(const Box2D &other) {
    Encapsulate(other.minX, other.minY);
    Encapsulate(other.maxX, other.maxY);
}

void Box2D::Translate(float dx, float dy) {
    minX += dx;
    minY += dy;
    maxX += dx;
    maxY += dy;
}

bool Box2D::Overlap(const Box2D &other, float &outPushX, float &outPushY) const {
    if (!Intersects(other)) {
        outPushX = 0.0f;
        outPushY = 0.0f;
        return false;
    }

    const float overlapLeft = maxX - other.minX;
    const float overlapRight = other.maxX - minX;
    const float overlapTop = maxY - other.minY;
    const float overlapBottom = other.maxY - minY;

    const float pushX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
    const float pushY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;

    if (std::abs(pushX) < std::abs(pushY)) {
        outPushX = pushX;
        outPushY = 0.0f;
    } else {
        outPushX = 0.0f;
        outPushY = pushY;
    }

    return true;
}

bool Box2D::operator==(const Box2D &other) const {
    return minX == other.minX && minY == other.minY &&
           maxX == other.maxX && maxY == other.maxY;
}

bool Box2D::operator!=(const Box2D &other) const {
    return !(*this == other);
}
