//
// Created by dlllibstdntc on 20.09.2026.
//

#ifndef FASTENGINE_HEALTHENTITY_H
#define FASTENGINE_HEALTHENTITY_H

#include <TheNightKnight/scripts/entity/LivingEntity.h>

class HealthEntity : public LivingEntity {
public:
    HealthEntity();

    RGBA GetHealthBarColor() override;
    void DrawHealthBar(const RGBA &color) override;
};


#endif //FASTENGINE_HEALTHENTITY_H
