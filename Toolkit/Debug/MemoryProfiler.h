#pragma once

#include <Windows.h>
#include <cstddef>
#include <cstdint>

class MemoryProfiler {
public:
    static void SetEnabled(bool value);
    static void SetOutputAllocate(const char* path);

    static void Initialize();
    static void Shutdown();
    static void Flush();

    static void RecordAllocate(void* address, std::size_t size);
    static void RecordDeallocate(void* address);

    static constexpr std::size_t MAX_FRAMES = 32;
    static constexpr std::size_t BUFFER_SIZE = 1024 * 1024;
};