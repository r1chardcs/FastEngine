#include "MeleeAIStates.h"
#include <cmath>

#include <Engine/Components/Transform.h>

#include "Engine/App.h"
#include "TheNightKnight/scenes/MainScene.h"

namespace {
    constexpr FLOAT kAttackCooldown = 1.0f;

    FLOAT Distance(const Vec2f& a, const Vec2f& b) {
        const FLOAT dx = a.x - b.x;
        const FLOAT dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }
}

SELF_PTR<EnemyState> MeleeSearchState::Update(VIEW_PTR<EnemyEntity> self) {
    const auto player = static_cast<MainScene*>(App::GetInstance().GetScene().operator->())->player;
    if (player && player->GetComponent<Transform>()->Position().DistanceTo(self->GetComponent<Transform>()->Position()) > self->GetSearchRadius()) {
        return nullptr;
    }
    const auto found = player;

    if (found && found->IsAlive()) {
        self->SetTarget(found.get());
        return MakeSelfPtr<MeleeChaseState>();
    }

    return nullptr;
}

SELF_PTR<EnemyState> MeleeChaseState::Update(VIEW_PTR<EnemyEntity> self) {
    const auto target = self->GetTarget();
    if (!target || !target->IsAlive()) {
        self->SetTarget(nullptr);
        return MakeSelfPtr<MeleeSearchState>();
    }

    const auto pos = self->GetComponent<Transform>()->Position().ToVec2();
    const auto targetPos = target->GetComponent<Transform>()->Position().ToVec2();
    const FLOAT dist = Distance(pos, targetPos);

    const auto specs = self->GetSpecifications();

    /* Трусость: при низком HP и приоритете "выжить" — убегаем вместо атаки,
       чем выше cowardice, тем дальше отбегаем за шаг. */
    const FLOAT healthRatio = self->GetMaxHealth() > 0
        ? static_cast<FLOAT>(self->GetHealth()) / static_cast<FLOAT>(self->GetMaxHealth())
        : 0.0f;

    constexpr FLOAT kLowHealthThreshold = 0.3f;
    if (specs->priority == EnemyEntity::Specifications::Priority::Survive
        && healthRatio < kLowHealthThreshold) {
        const FLOAT dx = pos.x - targetPos.x;
        const FLOAT dy = pos.y - targetPos.y;
        const FLOAT len = std::sqrt(dx * dx + dy * dy);
        if (len > 0.001f) {
            self->MoveTo({pos.x + (dx / len) * specs->cowardice,
                          pos.y + (dy / len) * specs->cowardice});
        }
        return nullptr;
    }

    if (dist <= self->GetAttackRange()) {
        return MakeSelfPtr<MeleeAttackState>();
    }

    if (dist > self->GetSearchRadius() * 1.5f) {
        self->SetTarget(nullptr);
        return MakeSelfPtr<MeleeSearchState>();
    }

    self->MoveTo(targetPos);
    return nullptr;
}

SELF_PTR<EnemyState> MeleeAttackState::Update(VIEW_PTR<EnemyEntity> self) {
    const auto target = self->GetTarget();
    if (!target || !target->IsAlive()) {
        self->SetTarget(nullptr);
        return MakeSelfPtr<MeleeSearchState>();
    }

    const auto pos = self->GetComponent<Transform>()->Position().ToVec2();
    const auto targetPos = target->GetComponent<Transform>()->Position().ToVec2();

    if (Distance(pos, targetPos) > self->GetAttackRange()) {
        return MakeSelfPtr<MeleeChaseState>();
    }

    cooldown -= static_cast<FLOAT>(self->GetDeltaTime());
    if (cooldown <= 0.0f) {
        target->Damage(static_cast<INT>(self->GetDamage()));
        cooldown = kAttackCooldown;
    }

    return nullptr;
}