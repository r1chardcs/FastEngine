//
// Created by dlllibstdntc on 17.09.2026.
//
#include "AnimationSystem.h"

#include "AnimationSystem.h"

AnimationSystem& AnimationSystem::GetInstance() {
    static AnimationSystem instance;
    return instance;
}

TweenHandle AnimationSystem::Add(GLOBAL_PTR<ITween> tween) {
    MUTEX_LOCK lock(mutex_tweens);
    const TweenHandle handle = next_handle++;
    tweens.push_back({handle, MOVE(tween)});
    return handle;
}

void AnimationSystem::Update(FLOAT deltaTime) {
    LIST<std::pair<TweenHandle, GLOBAL_PTR<ITween>>> snapshot;
    {
        MUTEX_LOCK lock(mutex_tweens);
        snapshot = tweens;
    }

    LIST<TweenHandle> finished;
    for (auto& entry : snapshot) {
        if (!entry.second->Update(deltaTime)) {
            finished.push_back(entry.first);
        }
    }

    if (!finished.empty()) {
        MUTEX_LOCK lock(mutex_tweens);
        tweens.remove_if([&finished](const std::pair<TweenHandle, GLOBAL_PTR<ITween>>& entry) {
            for (const auto& handle : finished) {
                if (entry.first == handle) return true;
            }
            return false;
        });
    }
}

void AnimationSystem::Stop(TweenHandle handle) {
    MUTEX_LOCK lock(mutex_tweens);
    for (auto& entry : tweens) {
        if (entry.first == handle) {
            entry.second->Stop();
            break;
        }
    }
}

void AnimationSystem::StopAll() {
    MUTEX_LOCK lock(mutex_tweens);
    tweens.clear();
}