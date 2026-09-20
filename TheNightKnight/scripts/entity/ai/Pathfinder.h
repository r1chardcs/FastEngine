#ifndef FASTENGINE_PATHFINDER_H
#define FASTENGINE_PATHFINDER_H

#include <Platform.h>
#include <Toolkit/Vector.h>

class Pathfinder {
public:
    Pathfinder() = default;

    Pathfinder(
        const VECTOR<STRING>& map,
        FLOAT blockSize = 1.0f
    );

    void Build(
        const VECTOR<STRING>& map,
        FLOAT blockSize = 1.0f
    );

    VECTOR<Vec2f> FindPath(
        const Vec2f& start,
        const Vec2f& target,
        FLOAT entityWidth = 0.58f,
        FLOAT entityHeight = 0.56f
    ) const;

    BOOL IsWalkable(INT x, INT y) const;

    Vec2f WorldToCell(const Vec2f& position) const;
    Vec2f CellToWorld(INT x, INT y) const;

    INT GetWidth() const;
    INT GetHeight() const;

private:
    struct Node {
        INT x = 0;
        INT y = 0;

        FLOAT g = 0.0f;
        FLOAT h = 0.0f;
        FLOAT f = 0.0f;

        INT parent = -1;
        BOOL opened = false;
        BOOL closed = false;
    };

    struct Cell {
        BOOL blocked = false;
    };

    INT GetIndex(INT x, INT y) const;

    FLOAT Heuristic(
        INT x1,
        INT y1,
        INT x2,
        INT y2
    ) const;

    BOOL CanFit(
        INT x,
        INT y,
        FLOAT entityWidth,
        FLOAT entityHeight
    ) const;

    BOOL CanMoveDiagonal(
        INT fromX,
        INT fromY,
        INT toX,
        INT toY,
        FLOAT entityWidth,
        FLOAT entityHeight
    ) const;

private:
    VECTOR<Cell> cells;

    INT width = 0;
    INT height = 0;

    FLOAT blockSize = 1.0f;

    FLOAT offsetX = 0.0f;
    FLOAT offsetY = 0.0f;
};

#endif