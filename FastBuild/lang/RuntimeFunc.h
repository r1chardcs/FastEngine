//
// Created by dlllibstdntc on 21.09.2026.
//

#ifndef FASTENGINE_RUNTIMEFUNC_H
#define FASTENGINE_RUNTIMEFUNC_H

#include <Platform.h>

#include "Toolkit/Debug/Logger.h"

class RuntimeFunc {
    STRING name;
public:
    RuntimeFunc(MOVE_PLEASE STRING name) : name(MOVE(name)) {}

    VIRTUAL void Run(const VECTOR<STRING> &arguments) = 0;

    static void Error(LITERAL msg);

    STRING GetName() {
        return name;
    }

    VIRTUAL ~RuntimeFunc() = default;
};

inline void RuntimeFunc::Error(const LITERAL msg) {
    LOGERR.Output(msg).Output("\n");
}

#endif //FASTENGINE_RUNTIMEFUNC_H
