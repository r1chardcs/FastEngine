//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_TRANSFORM_H
#define FASTENGINE_TRANSFORM_H

#include <Platform.h>

#include "../../Toolkit/Vector.h"
#include "../Component.h"
#include "../../Toolkit/Quat.h"

class Transform : public Component {
    Vec3f position;
    Vec3f size;
    Quat rotation;
public:
    Transform(const VIEW_PTR<GameObject> &self)
        : Component(self) {
    }

    void Render() override;

    Quat& Rotation();
    Vec3f& Position();
    Vec3f HalfPos() const;
    Vec3f& Size();

    CREATE_CLASS_ID(Transform)
};

#endif //FASTENGINE_TRANSFORM_H