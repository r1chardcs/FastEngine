//
// Created by dlllibstdntc on 19.09.2026.
//

#include "DarkSpawnEntityBlock.h"

#include "Engine/App.h"
#include "TheNightKnight/scripts/entity/enemy/Denuvo.h"

void DarkSpawnEntityBlock::DrawWorld() {
}

void DarkSpawnEntityBlock::Start() {
    ColorBlock::Start();

    const auto entity = MakeGlobalPtr<Denuvo>();
    entity->Teleport(*this);
    GetApp().GetScene()->AddGameObject(entity);
}
