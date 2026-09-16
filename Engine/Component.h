//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_COMPONENT_H
#define FASTENGINE_COMPONENT_H

#include <Platform.h>

class Component {
public:
    VIRTUAL ~Component() = default;
    VIRTUAL ClassID GetClassID() const = 0;

    void Update();
    void Render();
};

#endif //FASTENGINE_COMPONENT_H
