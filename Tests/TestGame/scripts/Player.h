//
// Created by dlllibstdntc on 23.09.2026.
//

#ifndef FASTENGINE_PLAYER_H
#define FASTENGINE_PLAYER_H

#include <Engine/GameObject.h>
#include <Engine/Objects/Rect.h>

#include "Engine/Components/Transform.h"

class Player : public Rect {
public:
    Player() {
        AddComponent<Transform>();
        
    }
};


#endif //FASTENGINE_PLAYER_H
