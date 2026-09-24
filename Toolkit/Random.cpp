//
// Created by dlllibstdntc on 19.09.2026.
//

#include "Random.h"


namespace toolkit::random {
    thread_local toolkit::random::LocalRandom g_localrandom{std::random_device{}()};

    LocalRandom::LocalRandom(const SIZE_T seed)
        : engine(seed), seed(seed) {
    }
    INT LocalRandom::RandInt(INT min, INT max) {
        if (min > max) std::swap(min, max);
        std::uniform_int_distribution<INT> dist(min, max);
        return dist(engine);
    }

    FLOAT LocalRandom::RandFloat(FLOAT min, FLOAT max) {
        if (min > max) std::swap(min, max);
        std::uniform_real_distribution<FLOAT> dist(min, max);
        return dist(engine);
    }

    DOUBLE LocalRandom::RandDouble(DOUBLE min, DOUBLE max) {
        if (min > max) std::swap(min, max);
        std::uniform_real_distribution<DOUBLE> dist(min, max);
        return dist(engine);
    }

    LONG LocalRandom::RandLong(LONG min, LONG max) {
        if (min > max) std::swap(min, max);
        std::uniform_int_distribution<LONG> dist(min, max);
        return dist(engine);
    }

    LONGLONG LocalRandom::RandLongLong(LONGLONG min, LONGLONG max) {
        if (min > max) std::swap(min, max);
        std::uniform_int_distribution<LONGLONG> dist(min, max);
        return dist(engine);
    }

    BOOL LocalRandom::RandBool() {
        std::uniform_int_distribution<INT> dist(0, 1);
        return dist(engine) == 1;
    }

    SIZE_T LocalRandom::GetSeed() const {
        return seed;
    }

    INT RandInt(const INT min,
                const INT max) {
        return g_localrandom.RandInt(min, max);
    }

    FLOAT RandFloat(const FLOAT min,
                    const FLOAT max) {
        return g_localrandom.RandFloat(min, max);
    }

    DOUBLE RandDouble(const DOUBLE min,
                      const DOUBLE max) {
        return g_localrandom.RandDouble(min, max);
    }

    LONG RandLong(const LONG min,
                  const LONG max) {
        return g_localrandom.RandLong(min, max);
    }

    LONGLONG RandLongLong(const LONGLONG min,
                          const LONGLONG max) {
        return g_localrandom.RandLongLong(min, max);
    }

    BOOL RandBool() {
        return g_localrandom.RandBool();
    }
}