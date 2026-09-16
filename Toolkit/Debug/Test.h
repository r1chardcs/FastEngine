//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_TEST_H
#define FASTENGINE_TEST_H
#include "Platform.h"

struct DebugObject {
    LITERAL file;
    LITERAL method;
    INT line;
};

namespace Test {
    void Fault(const char* test,
               const DebugObject& debug_object);
}

#define NEW_DEBUG_OBJECT DebugObject {__FILE__, __FUNCTION__, __LINE__}
#define TEST(expression) if(!(!(expression))) {Test::Fault(#expression, NEW_DEBUG_OBJECT);}

#endif //FASTENGINE_TEST_H
