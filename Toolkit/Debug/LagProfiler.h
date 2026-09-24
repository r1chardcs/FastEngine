//
// Created by dlllibstdntc on 19.09.2026.
//

#ifndef FASTENGINE_LAGPROFILER_H
#define FASTENGINE_LAGPROFILER_H

#include <Platform.h>

#include <chrono>

namespace toolkit::profiler::lag {
    struct ActionStats {
        LONG callCount = 0;
        DOUBLE totalTime = 0.0;  
        DOUBLE minTime   = -1.0;  
        DOUBLE maxTime   = 0.0;
        DOUBLE lastTime  = 0.0;

        DOUBLE Average() const {
            return callCount > 0 ? totalTime / static_cast<DOUBLE>(callCount) : 0.0;
        }
    };

    void Push(LITERAL action);
    void Pop();

    HASH_MAP<std::string, ActionStats> GetStats();

    void Reset();
    void PrintReport();

    class ScopedProfile {
    public:
        explicit ScopedProfile(LITERAL action) {
            Push(action);
        }
        ~ScopedProfile() {
            Pop();
        }
        ScopedProfile(const ScopedProfile&) = delete;
        ScopedProfile& operator=(const ScopedProfile&) = delete;
    };
}


#endif //FASTENGINE_LAGPROFILER_H
