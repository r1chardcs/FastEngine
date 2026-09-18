//
// Created by dlllibstdntc on 18.09.2026.
//

#include "ColorBlock.h"

void ColorBlock::DrawWorld() {
    AbstractBlock::DrawWorld();

    auto transform = GetComponent<Transform>();
    Render2D::DrawRect(
            transform->HalfPos().ToVec2(),
            transform->Size().ToVec2(), color
    );
}
