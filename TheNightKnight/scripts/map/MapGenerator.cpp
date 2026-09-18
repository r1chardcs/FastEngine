//
// Created by dlllibstdntc on 18.09.2026.
//

#include "MapGenerator.h"

VECTOR<GLOBAL_PTR<ColorBlock>> MapGenerator::CreateMap(FLOAT width, FLOAT height, Brush color) {
    VECTOR<GLOBAL_PTR<ColorBlock>> blocks;

    constexpr FLOAT blockSize = 1;

    const INT cols = static_cast<INT>(width / blockSize);
    const INT rows = static_cast<INT>(height / blockSize);

    const FLOAT offsetX = (cols - 1) * blockSize / 2.0f;
    const FLOAT offsetY = (rows - 1) * blockSize / 2.0f;

    for (INT y = 0; y < rows; y++) {
        for (INT x = 0; x < cols; x++) {
            const bool isBorder = (x == 0 || y == 0 || x == cols - 1 || y == rows - 1);
            if (!isBorder) {
                continue;
            }

            const auto block = MakeGlobalPtr<ColorBlock>(color);
            const auto transform = block->GetComponent<Transform>();
            transform->Position().x = x * blockSize - offsetX;
            transform->Position().y = y * blockSize - offsetY;

            blocks.push_back(block);
        }
    }

    return blocks;
}