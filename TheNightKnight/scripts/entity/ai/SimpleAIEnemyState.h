//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_SIMPLEAIENEMYSTATE_H
#define FASTENGINE_SIMPLEAIENEMYSTATE_H

#include "EnemyState.h"
#include "../EnemyEntity.h"

struct SearchState : EnemyState {
    SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) override;
};

struct ChaseState : EnemyState {
    SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) override;
};

struct AttackState : EnemyState {
    SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) override;
};

#endif //FASTENGINE_SIMPLEAIENEMYSTATE_H
