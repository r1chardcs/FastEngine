//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_SPRITE_H
#define FASTENGINE_SPRITE_H

#include "../Component.h"
#include "../Render2D.h"
#include "../Object.h"

class Sprite : public Component {
    NIL<Texture> texture;
    Recti srcRect;
    bool useSrcRect = false;
public:
    Sprite(const VIEW_PTR<GameObject> &self)
        : Component(self) {
    }

    void SetSourceRect(const Recti& rect);
    void ClearSourceRect();

    void SetTexture(const NIL<Texture> &texture_);
    void Render() override;

    CREATE_CLASS_ID(Sprite)
};


#endif //FASTENGINE_SPRITE_H
