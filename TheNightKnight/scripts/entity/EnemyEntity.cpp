//
// Created by dlllibstdntc on 18.09.2026.
//

#include "EnemyEntity.h"
#include <Toolkit/Random.h>

SELF_PTR<EnemyEntity::Specifications> EnemyEntity::Specifications::Random() {
    auto spec = MakeSelfPtr<Specifications>();

    spec->cowardice =  Random::RandFloat(1, 1.5);
    spec->strengthening = Random::RandFloat(1, 1.5);
    spec->celerity = Random::RandFloat(1, 1.5);
    spec->priority = Random::RandBool() == 0 ? Priority::Survive : Priority::Die;

    return spec;
}

VIEW_PTR<EnemyEntity::Specifications> EnemyEntity::GetSpecifications() const {
    return specifications.get();
}

void EnemyEntity::Update() {
    LivingEntity::Update();

    if (!current_state) {
        return;
    }

    if (auto next = current_state->Update(this)) {
        current_state->Exit(this);
        current_state = std::move(next);
        current_state->Enter(this);
    }
}

VIEW_PTR<LivingEntity> EnemyEntity::GetTarget() const { return target; }

void EnemyEntity::SetTarget(const VIEW_PTR<LivingEntity> value) { target = value; }
