//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_BOX2D_H
#define FASTENGINE_BOX2D_H

struct Box2D {
    float minX, minY, maxX, maxY;

    Box2D();
    Box2D(float minX_, float minY_, float maxX_, float maxY_);

    static Box2D FromCenterSize(float centerX, float centerY, float width, float height);

    float Width() const;
    float Height() const;
    float Area() const;
    float CenterX() const;
    float CenterY() const;
    bool IsEmpty() const;
    bool IsValid() const;
    bool Contains(float x, float y) const;
    bool Contains(const Box2D& other) const;
    bool Intersects(const Box2D& other) const;
    Box2D Intersection(const Box2D& other) const;
    Box2D Union(const Box2D& other) const;
    void Expand(float amount);
    void Encapsulate(float x, float y);
    void Encapsulate(const Box2D& other);
    void Translate(float dx, float dy);
    bool Overlap(const Box2D& other, float& outPushX, float& outPushY) const;

    bool operator==(const Box2D& other) const;
    bool operator!=(const Box2D& other) const;
};

#endif //FASTENGINE_BOX2D_H
