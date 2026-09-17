//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_CRASHDUMPER_H
#define FASTENGINE_CRASHDUMPER_H

#include <Platform.h>

struct StackFrame {
    STRING function_name;
    STRING module_name;
    STRING file_name;
    UINT line_number = 0;
    POINTER address = nullptr;
};

struct CrashContext {
    STRING exception_description;
    POINTER exception_address = nullptr;
    VECTOR<StackFrame> stack_trace;
};

namespace CrashDumper {
    void SetCallback(const FUNC<VOID(CrashContext)> &callback);
    void AttachHandler();
};


#endif //FASTENGINE_CRASHDUMPER_H
