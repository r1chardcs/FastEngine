//
// Created by dlllibstdntc on 19.09.2026.
//

#include "Random.h"

namespace {
    thread_local Random::LocalRandom g_localrandom{std::random_device{}()};
}

Random::LocalRandom::LocalRandom(const SIZE_T seed)
    : engine(seed), seed(seed) {
}
INT Random::LocalRandom::RandInt(INT min, INT max) {
    if (min > max) std::swap(min, max);
    std::uniform_int_distribution<INT> dist(min, max);
    return dist(engine);
}

FLOAT Random::LocalRandom::RandFloat(FLOAT min, FLOAT max) {
    if (min > max) std::swap(min, max);
    std::uniform_real_distribution<FLOAT> dist(min, max);
    return dist(engine);
}

DOUBLE Random::LocalRandom::RandDouble(DOUBLE min, DOUBLE max) {
    if (min > max) std::swap(min, max);
    std::uniform_real_distribution<DOUBLE> dist(min, max);
    return dist(engine);
}

LONG Random::LocalRandom::RandLong(LONG min, LONG max) {
    if (min > max) std::swap(min, max);
    std::uniform_int_distribution<LONG> dist(min, max);
    return dist(engine);
}

LONGLONG Random::LocalRandom::RandLongLong(LONGLONG min, LONGLONG max) {
    if (min > max) std::swap(min, max);
    std::uniform_int_distribution<LONGLONG> dist(min, max);
    return dist(engine);
}

BOOL Random::LocalRandom::RandBool() {
    std::uniform_int_distribution<INT> dist(0, 1);
    return dist(engine) == 1;
}

SIZE_T Random::LocalRandom::GetSeed() const {
    return seed;
}

INT Random::RandInt(INT min, INT max) {
    return g_localrandom.RandInt(min, max);
}

FLOAT Random::RandFloat(FLOAT min, FLOAT max) {
    return g_localrandom.RandFloat(min, max);
}

DOUBLE Random::RandDouble(DOUBLE min, DOUBLE max) {
    return g_localrandom.RandDouble(min, max);
}

LONG Random::RandLong(LONG min, LONG max) {
    return g_localrandom.RandLong(min, max);
}

LONGLONG Random::RandLongLong(LONGLONG min, LONGLONG max) {
    return g_localrandom.RandLongLong(min, max);
}

BOOL Random::RandBool() {
    return g_localrandom.RandBool();
}
