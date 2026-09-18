//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_SPRITE_H
#define FASTENGINE_SPRITE_H

#include "../Component.h"
#include "../Render2D.h"
#include "../GameObject.h"

class Sprite : public Component {
    NIL<Texture> texture;
    Recti srcRect;
    bool useSrcRect = false;
    BOOL flipX = false;
public:
    Sprite(const VIEW_PTR<GameObject> &self)
        : Component(self) {
    }
    void SetFlipX(BOOL flip);
    BOOL GetFlipX() const;

    void SetSourceRect(const Recti& rect);
    void ClearSourceRect();

    void SetTexture(const NIL<Texture> &texture_);
    void Render() override;

    public: static ClassID StaticClassID() {
        return 0x12;
    }

    ClassID GetClassID() const override {
        return 0x12;
    }
};


#endif //FASTENGINE_SPRITE_H
