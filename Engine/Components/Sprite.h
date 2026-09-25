//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_SPRITE_H
#define FASTENGINE_SPRITE_H

#include "../Component.h"
#include "../Render2D.h"
#include "../GameObject.h"

class Sprite : public Component {
    NIL<GLOBAL_PTR<Texture>> texture;
    Recti srcRect = {};
    bool useSrcRect = false;
    BOOL flipX = false;
    toolkit::RGBA tint = {.r = 1,.g = 1,.b = 1,.a = 1};
public:
    Sprite(const VIEW_PTR<GameObject> &self)
        : Component(self) {
    }
    toolkit::RGBA& Tint();

    void SetFlipX(BOOL flip);
    BOOL GetFlipX() const;

    void SetSourceRect(const Recti& rect);
    void ClearSourceRect();

    void SetTexture(const NIL<GLOBAL_PTR<Texture>> &texture_);
    void Render() override;

    public: static ClassID StaticClassID() {
        return 0x12;
    }

    ClassID GetClassID() const override {
        return 0x12;
    }
};


#endif //FASTENGINE_SPRITE_H
