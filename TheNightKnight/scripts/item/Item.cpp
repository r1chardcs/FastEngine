//
// Created by dlllibstdntc on 19.09.2026.
//

#include "Item.h"

VIEW_PTR<Texture> Item::GetTexture() const {
    return texture.get();
}
