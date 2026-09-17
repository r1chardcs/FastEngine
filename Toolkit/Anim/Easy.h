#ifndef FASTENGINE_EASE_H
#define FASTENGINE_EASE_H

#include <cmath>

namespace Ease {
    using Func = float(*)(float);

     float Linear(float t);
     float QuadIn(float t);
     float QuadOut(float t);
     float QuadInOut(float t);
     float CubicIn(float t);
     float CubicOut(float t);
     float CubicInOut(float t);
     float SineIn(float t);
     float SineOut(float t);
     float SineInOut(float t);
     float ExpoIn(float t);
     float ExpoOut(float t);
     float BackOut(float t);
     float ElasticOut(float t);
     float BounceOut(float t);
}

#endif