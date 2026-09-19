#ifndef FASTENGINE_ANIMATION_H
#define FASTENGINE_ANIMATION_H

#include <functional>

#include "AnimationSystem.h"
#include "Easy.h"
#include "Tween.h"

namespace Animation {

    inline TweenKey MakeKey(const void* owner, const char* property) {
        const LONG ptrHash = reinterpret_cast<unsigned long long>(owner);
        const LONG strHash = std::hash<STRING>{}(property);
        return ptrHash ^ (strHash + 0x9e3779b97f4a7c15ULL + (ptrHash << 6) + (ptrHash >> 2));
    }

    template <typename T>
    TweenHandle To(
        FUNC<T()> getter,
        FUNC<void(T)> setter,
        T endValue,
        FLOAT duration,
        Ease::Func easing = Ease::Linear,
        LoopMode loopMode = LoopMode::Once,
        FLOAT delay = 0.0f,
        FUNC<void()> onComplete = nullptr,
        TweenKey ownerKey = 0
    ) {
        auto tween = MakeGlobalPtr<Tween<T>>(
            MOVE(getter), MOVE(setter), MOVE(endValue), duration, easing, loopMode, delay
        );
        if (onComplete) {
            tween->SetOnComplete(MOVE(onComplete));
        }
        tween->ownerKey = ownerKey;

        GLOBAL_PTR<ITween> base = tween;
        return AnimationSystem::GetInstance().Add(base);
    }

    inline void Stop(TweenHandle handle) {
        AnimationSystem::GetInstance().Stop(handle);
    }

    inline void StopAll() {
        AnimationSystem::GetInstance().StopAll();
    }
}

#endif