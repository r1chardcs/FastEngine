//
// Created by dlllibstdntc on 21.09.2026.
//

#ifndef FASTENGINE_STATE_H
#define FASTENGINE_STATE_H

#include "SysBuild.h"

namespace State {
    inline GLOBAL_PTR<SysBuild> sys_build = NULLPTR;
    inline GLOBAL_PTR<CompilerInf> compiler_inf = NULLPTR;
    inline UINT jobs = 1;

    inline VECTOR<GLOBAL_PTR<BuildTarget>> all_targets;
    inline GLOBAL_PTR<BuildTarget> current_target;
    inline STRING projectName;
}

#endif //FASTENGINE_STATE_H
