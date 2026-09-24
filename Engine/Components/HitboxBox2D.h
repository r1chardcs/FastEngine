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
    toolkit::Box2D box;

    VIEW_PTR<Transform> transform = nullptr;

    BOOL solid = true;
    BOOL noMove = false;

    toolkit::Vec2f size;
    toolkit::Vec2f position;

public:
    explicit HitboxBox2D(const VIEW_PTR<GameObject> &self)
        : Component(self),
          size(0, 0),
          position(0, 0) {
    }

    bool isDebug = false;

    toolkit::Vec2f& Size();
    toolkit::Vec2f& Position();

    toolkit::Box2D& Box();

    BOOL& IsSolid();
    BOOL& NoMove();

    void Start() override;
    void Update() override;
    void Render(Type type) override;

    BOOL IsCollision(VIEW_PTR<GameObject> game_object) const;
    BOOL IsCollision(VIEW_PTR<HitboxBox2D> box2d) const;
    BOOL IsCollision(VIEW_PTR<toolkit::Box2D> box2d) const;

public:
    static ClassID StaticClassID() {
        return 0x11;
    }

    ClassID GetClassID() const override {
        return 0x11;
    }
};

#endif //FASTENGINE_HITBOXBOX2D_H
