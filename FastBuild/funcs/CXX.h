//
// Created by dlllibstdntc on 21.09.2026.
//

#ifndef FASTENGINE_COMPILER_H
#define FASTENGINE_COMPILER_H

#include  "../lang/RuntimeFunc.h"

class Compiler : public RuntimeFunc {
public:
    Compiler()
        : RuntimeFunc("print") {
    }

    void Run(const VECTOR<STRING> &arguments) override {
        if (arguments.empty()) {
            Error("Specify the type of assembly.");
            return;
        }


    }
};

#endif //FASTENGINE_COMPILER_H
