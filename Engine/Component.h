//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_COMPONENT_H
#define FASTENGINE_COMPONENT_H

#include <Platform.h>

class GameObject;

class Component {
protected:
    VIEW_PTR<GameObject> self;
public:
    enum class Type { Post, Pre };

    Component(VIEW_PTR<GameObject> self);

    VIRTUAL ~Component() = default;
    VIRTUAL ClassID GetClassID() const = 0;

    VIRTUAL void Start();
    VIRTUAL void Update();
    VIRTUAL void Render();
    VIRTUAL void Render(Type type);
    VIRTUAL void Shutdown();
};

#endif //FASTENGINE_COMPONENT_H
