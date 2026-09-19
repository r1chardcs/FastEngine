//
// Created by dlllibstdntc on 19.09.2026.
//

#ifndef FASTENGINE_DARKBLOCK_H
#define FASTENGINE_DARKBLOCK_H
#include <TheNightKnight/scripts/block/ColorBlock.h>

class DarkBlock : public ColorBlock {
public:
    explicit DarkBlock()
        : ColorBlock(Brush(RGBA(0.1, 0.1, 0.1, 1))) {
        AddTag("DarkBlock");
    }


};


#endif //FASTENGINE_DARKBLOCK_H
