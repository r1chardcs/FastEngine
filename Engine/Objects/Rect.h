//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_RECT_H
#define FASTENGINE_RECT_H

#include "../GameObject.h"
#include "../../Toolkit/Color.h"

class Rect : public GameObject {
    toolkit::Brush color = {
        {1,1,1,1}
    };
public:
    Rect();

    void SetColor(const toolkit::Brush& color_);

    toolkit::Brush GetColor();

    void DrawWorld() override;
};


#endif //FASTENGINE_RECT_H
