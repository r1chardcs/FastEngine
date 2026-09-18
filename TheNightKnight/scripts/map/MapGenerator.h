//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_MAPGENERATOR_H
#define FASTENGINE_MAPGENERATOR_H

#include "Platform.h"

#include "../block/ColorBlock.h"

namespace MapGenerator {
    VECTOR<GLOBAL_PTR<ColorBlock>> CreateMap(
        FLOAT width, FLOAT height, Brush color
    );
};


#endif //FASTENGINE_MAPGENERATOR_H
