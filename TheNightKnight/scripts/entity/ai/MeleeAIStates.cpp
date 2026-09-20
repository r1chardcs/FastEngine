#include "MeleeAIStates.h"
#include <cmath>

#include <Engine/Components/Transform.h>

#include "Engine/App.h"

namespace {
    constexpr FLOAT kAttackCooldown = 1.0f;
    constexpr LITERAL kVictimTag = "health";

    FLOAT Distance(const Vec2f& a, const Vec2f& b) {
        const FLOAT dx = a.x - b.x;
        const FLOAT dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    VIEW_PTR<LivingEntity> FindNearestVictim(VIEW_PTR<EnemyEntity> self, FLOAT searchRadius) {
        const auto selfTransform = self->GetComponent<Transform>();
        if (!selfTransform) return nullptr;

        const auto selfPos = selfTransform->Position().ToVec2();

        const auto candidates = App::GetInstance().GetGameObjectByTags(kVictimTag);

        VIEW_PTR<LivingEntity> nearest = nullptr;
        FLOAT nearestDist = searchRadius;

        for (const auto& obj : candidates) {
            if (!obj || obj.operator->() == self.operator->()) continue;
            if (!obj->IsActive()) continue;

            const auto living = dynamic_cast<LivingEntity*>(obj.operator->());
            if (!living || !living->IsAlive()) continue;

            const auto transform = living->GetComponent<Transform>();
            if (!transform) continue;

            const FLOAT dist = Distance(selfPos, transform->Position().ToVec2());
            if (dist <= nearestDist) {
                nearestDist = dist;
                nearest = living;
            }
        }

        return nearest;
    }
}

SELF_PTR<EnemyState> MeleeSearchState::Update(VIEW_PTR<EnemyEntity> self) {
    const auto found = FindNearestVictim(self, self->GetSearchRadius());

    if (found && found->IsAlive()) {
        self->SetTarget(found);
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

    const auto transform = self->GetComponent<Transform>();

    const auto pos = transform->Position().ToVec2();
    const auto targetPos = target->GetComponent<Transform>()->Position().ToVec2();

    const FLOAT dist = Distance(pos, targetPos);
    const auto specs = self->GetSpecifications();

    const FLOAT healthRatio = self->GetMaxHealth() > 0
        ? static_cast<FLOAT>(self->GetHealth()) /
          static_cast<FLOAT>(self->GetMaxHealth())
        : 0.0f;

    constexpr FLOAT kLowHealthThreshold = 0.3f;

    if (specs->priority == EnemyEntity::Specifications::Priority::Survive
        && healthRatio < kLowHealthThreshold) {

        const FLOAT searchRadius = self->GetSearchRadius();

        if (dist < searchRadius) {
            const FLOAT dx = pos.x - targetPos.x;
            const FLOAT dy = pos.y - targetPos.y;

            const FLOAT len = std::sqrt(dx * dx + dy * dy);

            if (len > 0.001f) {
                const FLOAT dirX = dx / len;
                const FLOAT dirY = dy / len;

                const FLOAT remaining = searchRadius - dist;
                const FLOAT moveDistance =
                    std::min(specs->cowardice, remaining);

                self->MoveTo({
                    pos.x + dirX * moveDistance,
                    pos.y + dirY * moveDistance
                });
            }
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