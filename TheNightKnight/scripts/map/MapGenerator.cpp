//
// Created by dlllibstdntc on 18.09.2026.
//

#include "MapGenerator.h"

#include <cstring>

#include <TheNightKnight/scripts/block/impl/dark/DarkBlock.h>
#include <TheNightKnight/scripts/block/impl/dark/DarkWinBlock.h>
#include <TheNightKnight/scripts/block/impl/dark/DarkSpawnEntityBlock.h>

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

VECTOR<GLOBAL_PTR<GameObject>> MapGenerator::ParseMap(LITERAL map) {
    VECTOR<GLOBAL_PTR<GameObject>> blocks;

    if (!map) {
        LOGWRN.Output("MapGenerator::ParseMap called with null map string\n");
        return blocks;
    }

    constexpr FLOAT blockSize = 1;

    /* Разбиваем строку на отдельные ряды по символу переноса строки,
       чтобы понять размер карты (rows/cols) до создания блоков —
       это нужно для правильного центрирования, как в CreateMap. */
    VECTOR<STRING> lines;
    {
        STRING current;
        for (const CHAR* p = map; *p; ++p) {
            if (*p == '\n') {
                lines.push_back(current);
                current.clear();
            } else {
                current += *p;
            }
        }
        if (!current.empty()) {
            lines.push_back(current);
        }
    }

    if (lines.empty()) {
        LOGWRN.Output("MapGenerator::ParseMap called with empty map string\n");
        return blocks;
    }

    const INT rows = static_cast<INT>(lines.size());

    INT cols = 0;
    for (const auto& line : lines) {
        cols = std::max(cols, static_cast<INT>(line.size()));
    }

    const FLOAT offsetX = (cols - 1) * blockSize / 2.0f;
    const FLOAT offsetY = (rows - 1) * blockSize / 2.0f;

    /* Первая строка текста — верхний ряд карты, но мировая ось Y растёт
       вверх (как в CreateMap: y увеличивается вместе с транслируемой
       позицией), поэтому переворачиваем индекс строки при вычислении
       мировой Y-координаты, иначе карта отрисуется "вверх ногами"
       относительно того, как она выглядит в текстовом виде. */
    for (INT lineIndex = 0; lineIndex < rows; lineIndex++) {
        const STRING& line = lines[lineIndex];
        const INT worldY = rows - 1 - lineIndex;

        for (INT x = 0; x < static_cast<INT>(line.size()); x++) {
            GLOBAL_PTR<GameObject> block;

            switch (line[x]) {
                case '#':
                    block = MakeGlobalPtr<DarkBlock>();
                    break;
                case '@':
                    block = MakeGlobalPtr<DarkWinBlock>();
                    break;
                case '^':
                    block = MakeGlobalPtr<DarkSpawnEntityBlock>();
                    break;
                default:
                    continue; // пустая клетка (пробел или что-то ещё) — ничего не создаём
            }

            const auto transform = block->GetComponent<Transform>();
            if (!transform) {
                LOGWRN.Output("ParseMap: block at (%d,%d) has no Transform component\n", x, lineIndex);
                continue;
            }

            transform->Position().x = x * blockSize - offsetX;
            transform->Position().y = worldY * blockSize - offsetY;

            blocks.push_back(block);
        }
    }

    return blocks;
}