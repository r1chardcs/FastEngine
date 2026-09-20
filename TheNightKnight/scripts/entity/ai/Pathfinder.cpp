#include "Pathfinder.h"

#include <algorithm>
#include <cmath>
#include <limits>

Pathfinder::Pathfinder(
    const VECTOR<STRING>& map,
    const FLOAT blockSize
) {
    Build(map, blockSize);
}

void Pathfinder::Build(
    const VECTOR<STRING>& map,
    const FLOAT blockSize
) {
    this->blockSize = blockSize;

    width = 0;
    height = static_cast<INT>(map.size());

    for (const auto& line : map) {
        width = std::max(
            width,
            static_cast<INT>(line.size())
        );
    }

    if (width <= 0 || height <= 0) {
        cells.clear();
        width = 0;
        height = 0;
        return;
    }

    cells.clear();
    cells.resize(width * height);

    offsetX = (width - 1) * blockSize / 2.0f;
    offsetY = (height - 1) * blockSize / 2.0f;

    for (INT y = 0; y < height; ++y) {
        const auto& line = map[y];

        for (INT x = 0; x < width; ++x) {
            const CHAR tile =
                x < static_cast<INT>(line.size())
                    ? line[x]
                    : ' ';

            auto& cell = cells[GetIndex(x, y)];

            cell.blocked =
                tile == '#' ||
                tile == '@';
        }
    }
}

INT Pathfinder::GetIndex(
    const INT x,
    const INT y
) const {
    return y * width + x;
}

BOOL Pathfinder::IsWalkable(
    const INT x,
    const INT y
) const {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }

    return !cells[GetIndex(x, y)].blocked;
}

Vec2f Pathfinder::WorldToCell(
    const Vec2f& position
) const {
    if (width <= 0 || height <= 0) {
        return {};
    }

    const FLOAT cellX =
        std::round((position.x + offsetX) / blockSize);

    const FLOAT cellY =
        std::round((position.y + offsetY) / blockSize);

    return {
        cellX,
        cellY
    };
}

Vec2f Pathfinder::CellToWorld(
    const INT x,
    const INT y
) const {
    return {
        x * blockSize - offsetX,
        y * blockSize - offsetY
    };
}

FLOAT Pathfinder::Heuristic(
    const INT x1,
    const INT y1,
    const INT x2,
    const INT y2
) const {
    const FLOAT dx =
        static_cast<FLOAT>(std::abs(x1 - x2));

    const FLOAT dy =
        static_cast<FLOAT>(std::abs(y1 - y2));

    return std::max(dx, dy);
}

BOOL Pathfinder::CanFit(
    const INT x,
    const INT y,
    const FLOAT entityWidth,
    const FLOAT entityHeight
) const {
    if (!IsWalkable(x, y)) {
        return false;
    }

    const FLOAT halfWidth = entityWidth * 0.5f;
    const FLOAT halfHeight = entityHeight * 0.5f;

    const FLOAT cellHalf = blockSize * 0.5f;

    const INT radiusX =
        static_cast<INT>(
            std::ceil(
                halfWidth / blockSize
            )
        );

    const INT radiusY =
        static_cast<INT>(
            std::ceil(
                halfHeight / blockSize
            )
        );

    for (INT yOffset = -radiusY; yOffset <= radiusY; ++yOffset) {
        for (INT xOffset = -radiusX; xOffset <= radiusX; ++xOffset) {
            const INT checkX = x + xOffset;
            const INT checkY = y + yOffset;

            if (checkX < 0 ||
                checkY < 0 ||
                checkX >= width ||
                checkY >= height) {
                return false;
            }

            if (!IsWalkable(checkX, checkY)) {
                if (xOffset == 0 && yOffset == 0) {
                    return false;
                }

                const FLOAT distanceX =
                    std::abs(
                        static_cast<FLOAT>(xOffset) * blockSize
                    );

                const FLOAT distanceY =
                    std::abs(
                        static_cast<FLOAT>(yOffset) * blockSize
                    );

                const FLOAT requiredX =
                    halfWidth + cellHalf;

                const FLOAT requiredY =
                    halfHeight + cellHalf;

                if (distanceX < requiredX &&
                    distanceY < requiredY) {
                    return false;
                }
            }
        }
    }

    return true;
}

BOOL Pathfinder::CanMoveDiagonal(
    const INT fromX,
    const INT fromY,
    const INT toX,
    const INT toY,
    const FLOAT entityWidth,
    const FLOAT entityHeight
) const {
    const INT dx = toX - fromX;
    const INT dy = toY - fromY;

    if (std::abs(dx) != 1 ||
        std::abs(dy) != 1) {
        return true;
    }

    if (!CanFit(
        fromX + dx,
        fromY,
        entityWidth,
        entityHeight)) {
        return false;
    }

    if (!CanFit(
        fromX,
        fromY + dy,
        entityWidth,
        entityHeight)) {
        return false;
    }

    return true;
}

VECTOR<Vec2f> Pathfinder::FindPath(
    const Vec2f& start,
    const Vec2f& target,
    const FLOAT entityWidth,
    const FLOAT entityHeight
) const {
    VECTOR<Vec2f> path;

    if (width <= 0 || height <= 0) {
        return path;
    }

    const Vec2f startCellPos = WorldToCell(start);
    const Vec2f targetCellPos = WorldToCell(target);

    const INT startX =
        static_cast<INT>(startCellPos.x);

    const INT startY =
        static_cast<INT>(startCellPos.y);

    const INT targetX =
        static_cast<INT>(targetCellPos.x);

    const INT targetY =
        static_cast<INT>(targetCellPos.y);

    if (startX < 0 ||
        startY < 0 ||
        startX >= width ||
        startY >= height) {
        return path;
    }

    if (targetX < 0 ||
        targetY < 0 ||
        targetX >= width ||
        targetY >= height) {
        return path;
    }

    if (!CanFit(
        startX,
        startY,
        entityWidth,
        entityHeight)) {
        return path;
    }

    if (!CanFit(
        targetX,
        targetY,
        entityWidth,
        entityHeight)) {
        return path;
    }

    const INT nodeCount = width * height;

    VECTOR<Node> nodes(nodeCount);

    for (INT y = 0; y < height; ++y) {
        for (INT x = 0; x < width; ++x) {
            auto& node = nodes[GetIndex(x, y)];

            node.x = x;
            node.y = y;
            node.g = std::numeric_limits<FLOAT>::max();
            node.h = 0.0f;
            node.f = std::numeric_limits<FLOAT>::max();
            node.parent = -1;
            node.opened = false;
            node.closed = false;
        }
    }

    const INT startIndex =
        GetIndex(startX, startY);

    const INT targetIndex =
        GetIndex(targetX, targetY);

    nodes[startIndex].g = 0.0f;
    nodes[startIndex].h =
        Heuristic(
            startX,
            startY,
            targetX,
            targetY
        );

    nodes[startIndex].f =
        nodes[startIndex].h;

    nodes[startIndex].opened = true;

    VECTOR<INT> openList;
    openList.push_back(startIndex);

    static constexpr INT directions[8][2] = {
        {-1, -1},
        { 0, -1},
        { 1, -1},
        {-1,  0},
        { 1,  0},
        {-1,  1},
        { 0,  1},
        { 1,  1}
    };

    while (!openList.empty()) {
        INT bestOpenPosition = 0;

        for (INT i = 1;
             i < static_cast<INT>(openList.size());
             ++i) {

            const Node& current =
                nodes[openList[i]];

            const Node& best =
                nodes[openList[bestOpenPosition]];

            if (current.f < best.f ||
                (current.f == best.f &&
                 current.h < best.h)) {
                bestOpenPosition = i;
            }
        }

        const INT currentIndex =
            openList[bestOpenPosition];

        openList.erase(
            openList.begin() + bestOpenPosition
        );

        Node& current =
            nodes[currentIndex];

        current.closed = true;

        if (currentIndex == targetIndex) {
            INT index = targetIndex;

            while (index != -1) {
                const Node& node = nodes[index];

                path.push_back(
                    CellToWorld(
                        node.x,
                        node.y
                    )
                );

                index = node.parent;
            }

            std::reverse(
                path.begin(),
                path.end()
            );

            if (!path.empty()) {
                path.erase(path.begin());
            }

            return path;
        }

        for (const auto& direction : directions) {
            const INT nextX =
                current.x + direction[0];

            const INT nextY =
                current.y + direction[1];

            if (!CanFit(
                nextX,
                nextY,
                entityWidth,
                entityHeight)) {
                continue;
            }

            if (!CanMoveDiagonal(
                current.x,
                current.y,
                nextX,
                nextY,
                entityWidth,
                entityHeight)) {
                continue;
            }

            const INT nextIndex =
                GetIndex(nextX, nextY);

            Node& next =
                nodes[nextIndex];

            if (next.closed) {
                continue;
            }

            const BOOL diagonal =
                direction[0] != 0 &&
                direction[1] != 0;

            const FLOAT movementCost =
                diagonal ? 1.41421356f : 1.0f;

            const FLOAT newG =
                current.g + movementCost;

            if (!next.opened ||
                newG < next.g) {

                next.parent = currentIndex;
                next.g = newG;

                next.h =
                    Heuristic(
                        nextX,
                        nextY,
                        targetX,
                        targetY
                    );

                next.f =
                    next.g + next.h;

                if (!next.opened) {
                    next.opened = true;
                    openList.push_back(nextIndex);
                }
            }
        }
    }

    return path;
}

INT Pathfinder::GetWidth() const {
    return width;
}

INT Pathfinder::GetHeight() const {
    return height;
}