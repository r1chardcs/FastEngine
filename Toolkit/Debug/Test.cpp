//
// Created by dlllibstdntc on 12.09.2026.
//

#include "Test.h"
#include "Logger.h"
#include "../IO/IO.h"

void Test::Fault(const char *test, const DebugObject &debug_object) {
    LOGERR.Output("Test Fault: '%s'\nFile: %s:%d\nFunction: '%s'\n", test, debug_object.file, debug_object.line, debug_object.method);
    const auto c
        = IO::InputChar("Finish or Continue? ['Y' - Continue]");

    if (c == 'y' || c == 'Y') {
        return;
    }
    CALL_ABORT;
}
