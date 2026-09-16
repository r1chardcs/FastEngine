//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_RECT_H
#define FASTENGINE_RECT_H

#include "../Object.h"
#include "../../Toolkit/Color.h"
#include  "../components/Transform.h"

class Rect : public GameObject {
    Brush color = {
        {1,1,1,1}
    };
public:
    Rect();

    void SetColor(const Brush& color_);
    Brush GetColor();

    void DrawWorld() override;
};


#endif //FASTENGINE_RECT_H
