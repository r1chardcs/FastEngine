//
// Created by dlllibstdntc on 18.09.2026.
//

#include "SimpleAIEnemyState.h"

SELF_PTR<EnemyState> SearchState::Update(VIEW_PTR<EnemyEntity> self) {
    auto found = /* поиск ближайшей LivingEntity в радиусе */ nullptr;
    if (found) {
        self->SetTarget(found);
        return MakeSelfPtr<ChaseState>();
    }
    return nullptr; // остаёмся в поиске
}

SELF_PTR<EnemyState> ChaseState::Update(VIEW_PTR<EnemyEntity> self) {
    auto target = self->GetTarget();
    if (!target || !target->IsAlive()) {
        return MakeSelfPtr<SearchState>();
    }
    // двигаться к target
    if (/* в радиусе атаки */ false) {
        return MakeSelfPtr<AttackState>();
    }
    return nullptr;
}

SELF_PTR<EnemyState> AttackState::Update(VIEW_PTR<EnemyEntity> self) {
    auto target = self->GetTarget();
    if (!target || !target->IsAlive()) {
        return MakeSelfPtr<SearchState>();
    }
    // атаковать target
    return nullptr;
}
