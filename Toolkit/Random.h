//
// Created by dlllibstdntc on 19.09.2026.
//

#ifndef FASTENGINE_RANDOM_H
#define FASTENGINE_RANDOM_H

#include <Platform.h>
#include <random>

namespace toolkit::random {
    class LocalRandom {
        std::mt19937_64 engine;
    public:
        explicit LocalRandom(SIZE_T seed);

        INT RandInt(INT min, INT max);
        FLOAT RandFloat(FLOAT min, FLOAT max);
        DOUBLE RandDouble(DOUBLE min, DOUBLE max);
        LONG RandLong(LONG min, LONG max);
        LONGLONG RandLongLong(LONGLONG min, LONGLONG max);
        BOOL RandBool();

        SIZE_T GetSeed() const;
    private:
        SIZE_T seed;
    };

    INT RandInt(INT min, INT max);
    FLOAT RandFloat(FLOAT min, FLOAT max);
    DOUBLE RandDouble(DOUBLE min, DOUBLE max);
    LONG RandLong(LONG min, LONG max);
    LONGLONG RandLongLong(LONGLONG min, LONGLONG max);
    BOOL RandBool();
};


#endif //FASTENGINE_RANDOM_H
