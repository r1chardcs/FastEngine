//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_ENEMYSTATE_H
#define FASTENGINE_ENEMYSTATE_H

#include <Platform.h>

class EnemyEntity;

struct EnemyState {
    virtual ~EnemyState() = default;

    VIRTUAL SELF_PTR<EnemyState> Update(VIEW_PTR<EnemyEntity> self) = 0;

    VIRTUAL VOID Enter(VIEW_PTR<EnemyEntity> self) {}
    VIRTUAL VOID Exit(VIEW_PTR<EnemyEntity> self) {}
};



#endif //FASTENGINE_ENEMYSTATE_H
