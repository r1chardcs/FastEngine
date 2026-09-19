#ifndef FASTENGINE_ITWEEN_H
#define FASTENGINE_ITWEEN_H
#include <Platform.h>

using TweenKey = LONG;

class ITween {
public:
    virtual ~ITween() = default;
    virtual BOOL Update(FLOAT deltaTime) = 0;
    virtual void Stop() = 0;
    virtual BOOL IsAlive() const = 0;

    TweenKey ownerKey = 0;
};

#endif