//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_COLORBLOCK_H
#define FASTENGINE_COLORBLOCK_H

#include "AbstractBlock.h"

class ColorBlock : public AbstractBlock {
public:
    ColorBlock(const Brush &color)
        : color(color) {
    }

    void DrawWorld() override;

private:
    Brush color;
};


#endif //FASTENGINE_COLORBLOCK_H
