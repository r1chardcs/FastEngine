//
// Created by dlllibstdntc on 18.09.2026.
//

#include "AbstractBlock.h"

AbstractBlock::AbstractBlock() {
    const auto transform = AddComponent<Transform>();
    AddComponent<HitboxBox2D>();

    transform->Size().x = 1;
    transform->Size().y = 1;
}

void AbstractBlock::Start() {
    GameObject::Start();

}
