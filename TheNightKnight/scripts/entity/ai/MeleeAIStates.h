#ifndef FASTENGINE_MELEEAISTATES_H
#define FASTENGINE_MELEEAISTATES_H

#include "EnemyState.h"
#include "../EnemyEntity.h"

struct MeleeSearchState : EnemyState {
    SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) override;
};

class MeleeChaseState : public EnemyState {
public:
    SELF_PTR<EnemyState> Update(
        VIEW_PTR<EnemyEntity> self
    ) override;

private:
    VECTOR<Vec2f> path;
    SIZE_T pathIndex = 0;

    FLOAT pathTimer = 0.0f;

    Vec2f lastTargetPosition = {};
    BOOL hasLastTargetPosition = false;
};

struct MeleeAttackState : EnemyState {
    FLOAT cooldown = 0.0f;
    VOID Enter(VIEW_PTR<EnemyEntity> self) override { cooldown = 0.0f; }
    SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) override;
};

#endif