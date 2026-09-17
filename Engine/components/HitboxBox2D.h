//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_HITBOXBOX2D_H
#define FASTENGINE_HITBOXBOX2D_H

#include "../Component.h"
#include "../Toolkit/Box2D.h"
#include "Toolkit/Vector.h"

class Transform;

class HitboxBox2D : public Component {
    Box2D box;
    VIEW_PTR<Transform> transform = nullptr;
    BOOL solid = true;
    Vec2f size;
public:
    explicit HitboxBox2D(const VIEW_PTR<GameObject> &self)
        : Component(self), size(0, 0) {
    }

    Vec2f& Size();
    Box2D& Box();
    BOOL& IsSolid();

    void Start() override;
    void Update() override;
    void Render() override;

    BOOL IsCollision(VIEW_PTR<GameObject> game_object) const;
    BOOL IsCollision(VIEW_PTR<HitboxBox2D> box2d) const;
    BOOL IsCollision(VIEW_PTR<Box2D> box2d) const;

    CREATE_CLASS_ID(HitboxBox2D);
};


#endif //FASTENGINE_HITBOXBOX2D_H
