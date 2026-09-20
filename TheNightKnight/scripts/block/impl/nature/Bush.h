//
// Created by dlllibstdntc on 20.09.2026.
//

#ifndef FASTENGINE_BUSH_H
#define FASTENGINE_BUSH_H

#include <TheNightKnight/scripts/block/SpriteBlock.h>

class Bush : public SpriteBlock {
public:
    explicit Bush()
        : SpriteBlock("assets/bush.png") {
        auto hb = GetComponent<HitboxBox2D>();
        hb->NoMove() = true;
        hb->IsSolid() = false;
    }
};


#endif //FASTENGINE_BUSH_H
