//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_PROFILER_H
#define FASTENGINE_PROFILER_H

#include <Platform.h>

namespace toolkit::profiler {
    struct RAM_MemoryInfo {
        unsigned long long workingSetSize;
        unsigned long long pageFileUsage;
        unsigned long long peakWorkingSize;
        unsigned long long peakFileUsage;
    };

    void Push(LITERAL format, ...);
    void Pop();

    BOOL IsEmpty();
    LITERAL GetProfiler();

    Err<RAM_MemoryInfo> GetRAMMemoyInfo();

}
#endif //FASTENGINE_PROFILER_H
