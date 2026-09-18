//
// Created by dlllibstdntc on 18.09.2026.
//

#ifndef FASTENGINE_FPSDRAW_H
#define FASTENGINE_FPSDRAW_H

#include <Engine/GameObject.h>
#include <Toolkit/Debug/LagProfiler.h>

class FPSOverlay : public GameObject {
    DOUBLE lagReportTimer = 0.0;
    std::vector<std::pair<std::string, LagProfiler::ActionStats>> cachedStats;
public:
    void DrawUI() override;
};


#endif //FASTENGINE_FPSDRAW_H
