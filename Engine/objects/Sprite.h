//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_SPRITE_H
#define FASTENGINE_SPRITE_H

#include "../Object.h"
#include "../Render2D.h"

class Sprite : public GameObject {
   Texture texture;
   Recti srcRect;
   bool useSrcRect = false;
public:
   Sprite(MOVE_PLEASE Texture texture);

   void SetSourceRect(const Recti& rect);
   void ClearSourceRect();

   Texture& GetTexture();

   void DrawWorld() override;
};


#endif //FASTENGINE_SPRITE_H
