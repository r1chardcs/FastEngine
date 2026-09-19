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

namespace ExceptionApi {
    struct StackFrame {
        static constexpr unsigned long long MAX_FUNCTION_NAME = 256;
        static constexpr unsigned long long MAX_MODULE_NAME = 256;
        static constexpr unsigned long long MAX_FILE_NAME = 512;

        CHAR function_name[MAX_FUNCTION_NAME] = {};
        CHAR module_name[MAX_MODULE_NAME] = {};
        CHAR file_name[MAX_FILE_NAME] = {};

        UINT line_number = 0;
        POINTER address = nullptr;
    };

    struct StackTrace {
        static constexpr unsigned long long MAX_FRAMES = 64;

        StackFrame frames[MAX_FRAMES] = {};
        unsigned long long count = 0;
    };

    STRING DescribeException(unsigned long code);
    StackTrace CaptureStackTraceCurrent();
}

namespace CrashDumper {
    void SetCallback(const FUNC<VOID(CrashContext)> &callback);
    void AttachHandler();
};


#endif //FASTENGINE_CRASHDUMPER_H
