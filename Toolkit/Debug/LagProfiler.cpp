//
// Created by dlllibstdntc on 19.09.2026.
//

#include "LagProfiler.h"

#include "../Toolkit/Debug/Logger.h"
#include <algorithm>

namespace {
    struct StackEntry {
        std::string action;
        std::chrono::high_resolution_clock::time_point start;
    };

    std::mutex mutex_stats;
    std::unordered_map<std::string, LagProfiler::ActionStats> stats;

    thread_local std::vector<StackEntry> stack;
}

void LagProfiler::Push(const char* action) {
    if (!action) return;

    stack.push_back({
        .action = std::string(action),
        .start = std::chrono::high_resolution_clock::now()
    });
}

void LagProfiler::Pop() {
    if (stack.empty()) {
        LOGWRN.Output("LagProfiler::Pop called without matching Push\n");
        return;
    }

    const auto [action, start] = std::move(stack.back());
    stack.pop_back();

    const auto end = std::chrono::high_resolution_clock::now();
    const DOUBLE elapsed = std::chrono::duration<DOUBLE>(end - start).count();

    MUTEX_LOCK lock(mutex_stats);
    auto &[callCount, totalTime,
    minTime, maxTime, lastTime] = stats[action];
    callCount++;
    totalTime += elapsed;
    lastTime = elapsed;
    if (minTime < 0.0 || elapsed < minTime) minTime = elapsed;
    if (elapsed > maxTime) maxTime = elapsed;
}

std::unordered_map<std::string, LagProfiler::ActionStats> LagProfiler::GetStats() {
    MUTEX_LOCK lock(mutex_stats);
    return stats;
}

void LagProfiler::Reset() {
    MUTEX_LOCK lock(mutex_stats);
    stats.clear();
}

void LagProfiler::PrintReport() {
    std::vector<std::pair<std::string, ActionStats>> sorted;
    {
        MUTEX_LOCK lock(mutex_stats);
        sorted.assign(stats.begin(), stats.end());
    }

    std::ranges::sort(sorted, [](const auto &a, const auto &b) {
        return a.second.totalTime > b.second.totalTime;
    });

    LOGWRN.Output("=== LagProfiler report ===\n");
    for (const auto &[name, s] : sorted) {
        LOGWRN.Output(
            "%-30s calls=%-6llu total=%.4fs avg=%.4fs min=%.4fs max=%.4fs\n",
            name.c_str(), s.callCount, s.totalTime, s.Average(), s.minTime, s.maxTime
        );
    }
}

