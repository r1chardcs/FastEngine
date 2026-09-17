#ifndef FASTENGINE_TWEEN_H
#define FASTENGINE_TWEEN_H

#include "ITween.h"
#include "Easy.h"
#include "Lerp.h"

enum class LoopMode {
    Once,
    Loop,
    PingPong
};

template <typename T>
class Tween : public ITween {
    FUNC<T()> getter;
    FUNC<void(T)> setter;

    T startValue{};
    T endValue;

    FLOAT duration;
    FLOAT delay;
    FLOAT elapsed = 0.0f;

    Ease::Func easing;
    LoopMode loopMode;
    BOOL reversed = false;
    BOOL alive = true;
    BOOL started = false;

    FUNC<void()> onComplete;

public:
    Tween(
        FUNC<T()> getter,
        FUNC<void(T)> setter,
        T endValue,
        FLOAT duration,
        Ease::Func easing = Ease::Linear,
        LoopMode loopMode = LoopMode::Once,
        FLOAT delay = 0.0f
    )
        : getter(MOVE(getter)),
          setter(MOVE(setter)),
          endValue(MOVE(endValue)),
          duration(duration),
          delay(delay),
          easing(easing),
          loopMode(loopMode) {
    }

    void SetOnComplete(FUNC<void()> callback) {
        onComplete = MOVE(callback);
    }

    BOOL Update(FLOAT deltaTime) override {
        if (!alive) return false;

        if (delay > 0.0f) {
            delay -= deltaTime;
            return true;
        }

        if (!started) {
            startValue = getter();
            started = true;
        }

        elapsed += deltaTime;
        FLOAT t = duration > 0.0f ? elapsed / duration : 1.0f;
        if (t > 1.0f) t = 1.0f;

        const FLOAT sampleT = reversed ? 1.0f - t : t;
        setter(Lerp<T>(startValue, endValue, easing(sampleT)));

        if (t >= 1.0f) {
            switch (loopMode) {
                case LoopMode::Once:
                    alive = false;
                    if (onComplete) onComplete();
                    return false;

                case LoopMode::Loop:
                    elapsed = 0.0f;
                    if (onComplete) onComplete();
                    return true;

                case LoopMode::PingPong:
                    elapsed = 0.0f;
                    reversed = !reversed;
                    if (onComplete) onComplete();
                    return true;
            }
        }

        return true;
    }

    void Stop() override {
        alive = false;
    }

    BOOL IsAlive() const override {
        return alive;
    }
};

#endif