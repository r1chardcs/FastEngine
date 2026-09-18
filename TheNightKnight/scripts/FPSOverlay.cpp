//
// Created by dlllibstdntc on 18.09.2026.
//

#include "FPSOverlay.h"

#include <algorithm>
#include <vector>

#include "Engine/App.h"
#include "Engine/UIContext.h"
#include "Toolkit/Debug/Profiler.h"
#include "Toolkit/Debug/LagProfiler.h"

void FPSOverlay::DrawUI() {
    GameObject::DrawUI();
    UIContext ctx(GetRenderSystem());
    const auto font = Render2D::GetFont("C:/Windows/Fonts/Arial.ttf", 32).res.get();

    char buf[128];
    const auto ram = Profiler::GetRAMMemoyInfo();

    if (ram.err) {
        sprintf(buf, "FPS: %.0f  RAM: %s", GetApp().GetFPS(), ram.err);
    } else {
        const auto& m = ram.res;
        sprintf(buf, "FPS: %.0f  WS: %llu MB  PF: %llu MB  Peak: %llu/%llu MB GameObjects: %d",
                GetApp().GetFPS(),
                m.workingSetSize / 1024 / 1024,
                m.pageFileUsage  / 1024 / 1024,
                m.peakWorkingSize / 1024 / 1024,
                m.peakFileUsage   / 1024 / 1024,
                GetApp().GetScene()->GetGameObjectSize());
    }
    ctx.Text(Layout::UP_LEFT, font, buf, {5, 5}, {1, 1, 1, 1}, 0.5);

    lagReportTimer += GetDeltaTime();
    if (lagReportTimer >= 1.0) {
        cachedStats.clear();
        for (const auto& [name, s] : LagProfiler::GetStats()) {
            cachedStats.push_back({name, s});
        }
        std::sort(cachedStats.begin(), cachedStats.end(), [](const auto& a, const auto& b) {
            return a.second.totalTime > b.second.totalTime;
        });
        LagProfiler::Reset();
        lagReportTimer = 0.0;
    }

    if (cachedStats.empty()) {
        return;
    }

    constexpr INT maxRows = 8;
    const INT rows = std::min<INT>(maxRows, static_cast<INT>(cachedStats.size()));

    constexpr INT rowHeight = 22;
    constexpr INT panelWidth = 340;
    constexpr INT panelTop = 30;
    const INT panelHeight = rowHeight * (rows + 1) + 12;

    ctx.Rect(Layout::UP_LEFT, {5, panelTop}, {panelWidth, panelHeight}, {0, 0, 0, 0.55f});
    ctx.Border(Layout::UP_LEFT, {5, panelTop}, {panelWidth, panelHeight}, {1, 1, 1, 0.15f});

    ctx.Text(Layout::UP_LEFT, font, "LP Lag Trace",
              {12, panelTop + 6}, {1, 1, 0.4f, 1}, 0.4f);

    for (INT i = 0; i < rows; i++) {
        const auto& [name, s] = cachedStats[i];

        char row[96];
        sprintf(row, "%-22s %6.2fms avg  x%ld",
                name.c_str(), s.Average() * 1000.0, s.callCount);

        RGBA color = {.r = 0.4f, .g = 1.0f, .b = 0.4f, .a = 1};
        if (s.Average() > 0.008) color = {.r = 1.0f, .g = 1.0f, .b = 0.3f, .a = 1};
        if (s.Average() > 0.016) color = {.r = 1.0f, .g = 0.35f, .b = 0.35f, .a = 1};

        ctx.Text(Layout::UP_LEFT, font, row,
                  {12, panelTop + 6 + rowHeight * (i + 1)}, color, 0.4f);
    }
}