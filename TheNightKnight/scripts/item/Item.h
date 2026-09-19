//
// Created by dlllibstdntc on 19.09.2026.
//

#ifndef FASTENGINE_ITEM_H
#define FASTENGINE_ITEM_H

#include <Engine/GameObject.h>

class Item : public GameObject {
    GLOBAL_PTR<Texture> texture= NULLPTR;
public:
    explicit Item(const LITERAL texture_path)
        : texture_path(texture_path) {
        texture = GetRenderSystem()->LoadTextureSync(texture_path);
    }

    VIEW_PTR<Texture> GetTexture() const;
private:
    LITERAL texture_path;
};


#endif //FASTENGINE_ITEM_H
