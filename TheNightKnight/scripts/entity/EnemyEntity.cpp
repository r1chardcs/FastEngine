//
// Created by dlllibstdntc on 18.09.2026.
//

#include "EnemyEntity.h"
#include <random>

SELF_PTR<EnemyEntity::Specifications> EnemyEntity::Specifications::Random() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<FLOAT> multiplierDist(1.0f, 1.5f);
    static std::uniform_int_distribution<INT> priorityDist(0, 1);

    auto spec = MakeSelfPtr<Specifications>();

    spec->cowardice = multiplierDist(gen);
    spec->strengthening = multiplierDist(gen);
    spec->celerity = multiplierDist(gen);
    spec->priority = priorityDist(gen) == 0 ? Priority::Survive : Priority::Die;

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
