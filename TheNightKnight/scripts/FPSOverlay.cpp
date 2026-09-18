//
// Created by dlllibstdntc on 18.09.2026.
//

#include "FPSOverlay.h"

#include "Engine/App.h"
#include "Engine/UIContext.h"
#include "Toolkit/Debug/Profiler.h"

void FPSOverlay::DrawUI() {
    GameObject::DrawUI();
    UIContext ctx(GetRenderSystem());
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
    ctx.Text(
        Layout::UP_LEFT, Render2D::GetFont("C:/Windows/Fonts/Arial.ttf", 32).res.get(), buf, {5, 5}, {1, 1, 1, 1}, 0.5);
}
