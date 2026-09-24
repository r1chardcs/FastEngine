//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_TEST_H
#define FASTENGINE_TEST_H
#include "Platform.h"

namespace toolkit {
    struct DebugObject {
        LITERAL file;
        LITERAL method;
        INT line;
    };

    namespace test {
        void Fault(const char* test,
                   const DebugObject& debug_object);
    }
}
#define NEW_DEBUG_OBJECT toolkit::DebugObject {__FILE__, __FUNCTION__, __LINE__}
#define TEST(expression) if(!(!(expression))) {toolkit::test::Fault(#expression, NEW_DEBUG_OBJECT);}
#define TEST_THIS(x) TEST(not static_cast<char>(reinterpret_cast<unsigned long long>(this)) && #x)

#endif //FASTENGINE_TEST_H
