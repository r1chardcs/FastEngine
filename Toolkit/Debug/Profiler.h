//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_PROFILER_H
#define FASTENGINE_PROFILER_H

#include <Platform.h>

namespace Profiler {
    void Push(LITERAL format, ...);
    void Pop();

    BOOL IsEmpty();
    LITERAL GetProfiler();
}

#endif //FASTENGINE_PROFILER_H
