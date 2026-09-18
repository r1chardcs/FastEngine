#ifndef FASTENGINE_MELEEAISTATES_H
#define FASTENGINE_MELEEAISTATES_H

#include "EnemyState.h"
#include "../EnemyEntity.h"

struct MeleeSearchState : EnemyState {
    SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) override;
};

struct MeleeChaseState : EnemyState {
    SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) override;
};

struct MeleeAttackState : EnemyState {
    FLOAT cooldown = 0.0f;
    VOID Enter(VIEW_PTR<EnemyEntity> self) override { cooldown = 0.0f; }
    SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) override;
};

#endif