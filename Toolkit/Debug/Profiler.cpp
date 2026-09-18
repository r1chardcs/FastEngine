//
// Created by dlllibstdntc on 12.09.2026.
//

#include "Profiler.h"

#include <cstring>
#include <stdarg.h>
#include <Windows.h>
#include <psapi.h>

static char buf[256];

void Profiler::Push(LITERAL format, ...) {
    if (!IsEmpty()) Pop();

    va_list x;
    va_start(x, format);
    vsprintf_s(buf, 256, format, x);
    va_end(x);
}

void Profiler::Pop() {
    memset(buf, 0, 255);
}

BOOL Profiler::IsEmpty() {
    return buf[0] == 0;
}

LITERAL Profiler::GetProfiler() {
    if (IsEmpty()) return nullptr;

    return buf;
}

Err<RAM_MemoryInfo> Profiler::GetRAMMemoyInfo() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        RAM_MemoryInfo ram = {};
        ram.pageFileUsage = pmc.PagefileUsage;
        ram.peakFileUsage = pmc.PeakPagefileUsage;
        ram.workingSetSize = pmc.WorkingSetSize;
        ram.peakWorkingSize = pmc.PeakWorkingSetSize;
        return {.res = ram, .err = nullptr};
    }
    return {.res = {}, .err = "Access denied"};
}
