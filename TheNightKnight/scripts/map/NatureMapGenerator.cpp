//
// Created by dlllibstdntc on 20.09.2026.
//

#include "NatureMapGenerator.h"

#include <TheNightKnight/scripts/block/impl/nature/Bush.h>

#include "Engine/App.h"
#include "Toolkit/Random.h"

void NatureMapGenerator::generate(FLOAT size) {
    constexpr FLOAT blockSize = 1;
    constexpr FLOAT exclusionRadius = 2.5f; // зона вокруг (0,0), где блоков быть не должно

    const INT cols = static_cast<INT>(size / blockSize);
    const INT rows = static_cast<INT>(size / blockSize);

    const FLOAT offsetX = (cols - 1) * blockSize / 2.0f;
    const FLOAT offsetY = (rows - 1) * blockSize / 2.0f;

    for (INT y = 0; y < rows; y++) {
        for (INT x = 0; x < cols; x++) {
            const FLOAT worldX = x * blockSize - offsetX;
            const FLOAT worldY = y * blockSize - offsetY;

            /* Пропускаем клетки внутри квадрата [-5, 5] по обеим осям
               вокруг центра карты (0,0). */
            if (worldX >= -exclusionRadius && worldX <= exclusionRadius &&
                worldY >= -exclusionRadius && worldY <= exclusionRadius) {
                continue;
                }

            if (Random::RandInt(0, 100) % 5) continue;

            const auto block = MakeGlobalPtr<Bush>();
            const auto transform = block->GetComponent<Transform>();
            transform->Position().x = worldX;
            transform->Position().y = worldY;

            App::GetInstance().GetScene()->AddGameObject(block);
        }
    }
}