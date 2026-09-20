//
// Created by dlllibstdntc on 19.09.2026.
//

#ifndef FASTENGINE_DARKSPAWNENTITYBLOCK_H
#define FASTENGINE_DARKSPAWNENTITYBLOCK_H

#include <TheNightKnight/scripts/block/ColorBlock.h>

class DarkSpawnEntityBlock : public ColorBlock {
public:
    explicit DarkSpawnEntityBlock()
        : ColorBlock(Brush(RGBA(0, 0, 0, 0))) {
        AddTag("DarkBlock_SpawnEntity");
        GetComponent<HitboxBox2D>()->IsSolid() = false;
    }

    void DrawWorld() override;
    void Start() override;
};


#endif //FASTENGINE_DARKSPAWNENTITYBLOCK_H
