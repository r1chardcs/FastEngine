//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_ENEMYENTITY_H
#define FASTENGINE_ENEMYENTITY_H


#include "LivingEntity.h"
#include "ai/EnemyState.h"

class EnemyEntity;

struct SearchStrategy {
    virtual ~SearchStrategy() = default;
    VIRTUAL VIEW_PTR<LivingEntity> Search(VIEW_PTR<EnemyEntity> self) = 0;
};

struct AttackStrategy {
    virtual ~AttackStrategy() = default;
    VIRTUAL BOOL Attack(VIEW_PTR<EnemyEntity> self, VIEW_PTR<LivingEntity> target) = 0;
};



class EnemyEntity : public LivingEntity
{
public:
    struct Specifications {
        enum class Priority { Survive, Die };
        FLOAT strengthening = 1;
        FLOAT cowardice     = 1;
        FLOAT celerity      = 1;
        Priority priority   = Priority::Die;

        static SELF_PTR<Specifications> Random();
    };
private:
    VIEW_PTR<LivingEntity> target = NULLPTR;
    SELF_PTR<EnemyState> current_state;
    SELF_PTR<Specifications> specifications;
public:
    EnemyEntity(const INT maxHealth, const FLOAT speed,
        MOVE_PLEASE SELF_PTR<EnemyState> initialState,
        MOVE_PLEASE SELF_PTR<Specifications> specs = nullptr)
        : LivingEntity(maxHealth, speed)
        , current_state(MOVE(initialState))
        , specifications(specs ? MOVE(specs) : Specifications::Random()) {
        if (current_state) {
            current_state->Enter(this);
        }
    }

    VIEW_PTR<Specifications> GetSpecifications() const;
    void Update() override;
    VIEW_PTR<LivingEntity> GetTarget() const;
    void SetTarget(VIEW_PTR<LivingEntity> value);

    VIRTUAL FLOAT GetSearchRadius() { return 5.0f * GetSpecifications()->celerity; }
    VIRTUAL FLOAT GetDamage() { return 5.0f * GetSpecifications()->strengthening; }
    VIRTUAL FLOAT GetAttackRange() { return 1.2f; }
};

#endif //FASTENGINE_ENEMYENTITY_H
