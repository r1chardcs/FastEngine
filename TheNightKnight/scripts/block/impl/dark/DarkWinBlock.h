//
// Created by dlllibstdntc on 19.09.2026.
//

#ifndef FASTENGINE_DARKWINBLOCK_H
#define FASTENGINE_DARKWINBLOCK_H
#include <TheNightKnight/scripts/block/ColorBlock.h>

class DarkWinBlock  : public ColorBlock {
public:
    explicit DarkWinBlock()
        : ColorBlock(Brush(RGBA(0.5, 0.5, 0.1, 1))) {
        AddTag("DarkBlock_Win");
    }

};


#endif //FASTENGINE_DARKWINBLOCK_H
