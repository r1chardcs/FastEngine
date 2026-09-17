#ifndef FASTENGINE_ANIMATION_H
#define FASTENGINE_ANIMATION_H

#include "AnimationSystem.h"
#include "Easy.h"
#include "Tween.h"

namespace Animation {
    template <typename T>
 TweenHandle To(
     FUNC<T()> getter,
     FUNC<void(T)> setter,
     T endValue,
     FLOAT duration,
     Ease::Func easing = Ease::Linear,
     LoopMode loopMode = LoopMode::Once,
     FLOAT delay = 0.0f,
     FUNC<void()> onComplete = nullptr
 ) {
        auto tween = MakeGlobalPtr<Tween<T>>(
            MOVE(getter), MOVE(setter), MOVE(endValue), duration, easing, loopMode, delay
        );
        if (onComplete) {
            tween->SetOnComplete(MOVE(onComplete));
        }
        GLOBAL_PTR<ITween> base = tween;
        return AnimationSystem::GetInstance().Add(base);
    }

    template <typename T>
    TweenHandle To(
        FUNC<T()> getter,
        FUNC<void(T)> setter,
        T endValue,
        FLOAT duration,
        Ease::Func easing = Ease::Linear,
        LoopMode loopMode = LoopMode::Once,
        FLOAT delay = 0.0f
    ) {
        GLOBAL_PTR<ITween> tween = MakeGlobalPtr<Tween<T>>(
            MOVE(getter), MOVE(setter), MOVE(endValue), duration, easing, loopMode, delay
        );
        return AnimationSystem::GetInstance().Add(tween);
    }

    inline void Stop(TweenHandle handle) {
        AnimationSystem::GetInstance().Stop(handle);
    }

    inline void StopAll() {
        AnimationSystem::GetInstance().StopAll();
    }
}

#endif