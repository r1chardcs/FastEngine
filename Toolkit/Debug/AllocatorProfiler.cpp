#include "AllocatorProfiler.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "SelfCrashDumper.h"

namespace {
    struct AllocationEvent {
        enum class Type : uint8_t {
            Allocate,
            Deallocate
        };

        Type type;
        uint8_t reserved[7];

        void* address;
        std::size_t size;

        uint32_t frameCount;
        void* frames[toolkit::profiler::AllocatorProfiler::MAX_FRAMES];
    };

    alignas(64)
    AllocationEvent g_events[
        toolkit::profiler::AllocatorProfiler::BUFFER_SIZE /
        sizeof(AllocationEvent)
    ];

    constexpr std::size_t g_eventCount =
        std::size(g_events);

    volatile LONG64 g_writeIndex = 0;
    volatile LONG64 g_readIndex = 0;

    bool g_isEnabled = false;

    const char* g_outputFile =
        "memory_allocation.dmp";

    HANDLE g_flushEvent = nullptr;
    HANDLE g_flushThread = nullptr;

    DWORD WINAPI FlushThreadProc(void*) {
        while (g_isEnabled) {
            WaitForSingleObject(g_flushEvent, 100);

            toolkit::profiler::AllocatorProfiler::Flush();
        }

        toolkit::profiler::AllocatorProfiler::Flush();

        return 0;
    }
}

void toolkit::profiler::AllocatorProfiler::Initialize() {
    if (g_flushEvent)
        return;

    g_flushEvent = CreateEventA(
        nullptr,
        FALSE,
        FALSE,
        nullptr
    );

    if (!g_flushEvent)
        return;

    g_flushThread = CreateThread(
        nullptr,
        0,
        FlushThreadProc,
        nullptr,
        0,
        nullptr
    );
}

void toolkit::profiler::AllocatorProfiler::Shutdown() {
    g_isEnabled = false;

    if (g_flushEvent)
        SetEvent(g_flushEvent);

    if (g_flushThread) {
        WaitForSingleObject(
            g_flushThread,
            INFINITE
        );

        CloseHandle(g_flushThread);
        g_flushThread = nullptr;
    }

    if (g_flushEvent) {
        CloseHandle(g_flushEvent);
        g_flushEvent = nullptr;
    }

    Flush();
}

void toolkit::profiler::AllocatorProfiler::SetEnabled(bool value) {
    g_isEnabled = value;

    if (value && !g_flushEvent)
        Initialize();
}

void toolkit::profiler::AllocatorProfiler::SetOutputAllocate(const char* path) {
    if (path)
        g_outputFile = path;
}

void toolkit::profiler::AllocatorProfiler::RecordAllocate(
    void* address,
    std::size_t size
) {
    if (!g_isEnabled)
        return;

    const LONG64 write =
        InterlockedIncrement64(&g_writeIndex) - 1;

    const LONG64 read =
        InterlockedCompareExchange64(
            &g_readIndex,
            0,
            0
        );

    if (write - read >= static_cast<LONG64>(g_eventCount)) {
        return;
    }

    AllocationEvent& event =
        g_events[write % g_eventCount];

    event.type =
        AllocationEvent::Type::Allocate;

    event.address = address;
    event.size = size;

    auto stacktrace =
        no_new::CaptureStackTraceCurrent();

    const std::size_t count =
        stacktrace.count < MAX_FRAMES
            ? stacktrace.count
            : MAX_FRAMES;

    event.frameCount =
        static_cast<uint32_t>(count);

    for (std::size_t i = 0; i < count; ++i) {
        event.frames[i] =
            stacktrace.frames[i].address;
    }

    SetEvent(g_flushEvent);
}

void toolkit::profiler::AllocatorProfiler::RecordDeallocate(
    void* address
) {
    if (!g_isEnabled || !address)
        return;

    const LONG64 write =
        InterlockedIncrement64(&g_writeIndex) - 1;

    const LONG64 read =
        InterlockedCompareExchange64(
            &g_readIndex,
            0,
            0
        );

    if (write - read >= static_cast<LONG64>(g_eventCount)) {
        return;
    }

    AllocationEvent& event =
        g_events[write % g_eventCount];

    event.type =
        AllocationEvent::Type::Deallocate;

    event.address = address;
    event.size = 0;
    event.frameCount = 0;
}

void toolkit::profiler::AllocatorProfiler::Flush() {
    static bool initialized = false;

    FILE* file = nullptr;

    if (!initialized) {
        if (fopen_s(
            &file,
            g_outputFile,
            "w"
        ) != 0 || !file) {
            return;
        }

        fprintf(
            file,
            "=== MEMORY PROFILER ===\n\n"
        );

        initialized = true;
    } else {
        if (fopen_s(
            &file,
            g_outputFile,
            "a"
        ) != 0 || !file) {
            return;
        }
    }

    const LONG64 write =
        InterlockedCompareExchange64(
            &g_writeIndex,
            0,
            0
        );

    LONG64 read =
        InterlockedCompareExchange64(
            &g_readIndex,
            0,
            0
        );

    while (read < write) {
        const AllocationEvent& event =
            g_events[read % g_eventCount];

        if (event.type ==
            AllocationEvent::Type::Allocate) {

            fprintf(
                file,
                "\nALLOCATE %zu bytes at %p\n",
                event.size,
                event.address
            );

            for (uint32_t i = 0;
                 i < event.frameCount;
                 ++i) {

                fprintf(
                    file,
                    "  #%u %p\n",
                    i,
                    event.frames[i]
                );
            }
        } else {
            fprintf(
                file,
                "DEALLOCATE %p\n",
                event.address
            );
        }

        ++read;
    }

    InterlockedExchange64(
        &g_readIndex,
        read
    );

    fclose(file);
}