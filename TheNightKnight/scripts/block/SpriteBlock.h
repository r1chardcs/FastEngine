//
// Created by dlllibstdntc on 19.09.2026.
//

#ifndef FASTENGINE_SPRITEBLOCK_H
#define FASTENGINE_SPRITEBLOCK_H
#include "AbstractBlock.h"
#include "Engine/Components/Sprite.h"

class SpriteBlock : public AbstractBlock {
    GLOBAL_PTR<Texture> texture;
public:
    explicit SpriteBlock(LITERAL path)
        : path(path) {
        texture = GetRenderSystem()->LoadTextureSync(path);
        const auto sprite = AddComponent<Sprite>();
        sprite->SetTexture(texture);
    }

    void DrawWorld() override;

private:
    LITERAL path;


};


#endif //FASTENGINE_SPRITEBLOCK_H
