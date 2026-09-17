#ifndef FASTENGINE_ANIMATIONSYSTEM_H
#define FASTENGINE_ANIMATIONSYSTEM_H

#include <cstdint>
#include <utility>

#include "ITween.h"

using TweenHandle = std::uint64_t;

class AnimationSystem {
    LIST<std::pair<TweenHandle, GLOBAL_PTR<ITween>>> tweens;
    mutable MUTEX mutex_tweens;
    TweenHandle next_handle = 1;

public:
    static AnimationSystem& GetInstance();

    void Update(FLOAT deltaTime);

    TweenHandle Add(GLOBAL_PTR<ITween> tween);
    void Stop(TweenHandle handle);
    void StopAll();
};

#endif