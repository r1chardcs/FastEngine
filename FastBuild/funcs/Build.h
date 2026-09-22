//
// Created by dlllibstdntc on 21.09.2026.
//

#ifndef FASTENGINE_BUILD_H
#define FASTENGINE_BUILD_H
#include  "../lang/RuntimeFunc.h"
#include "FastBuild/State.h"

class Build : public RuntimeFunc {
public:
    Build() : RuntimeFunc("build") {

    }

    void Run(const VECTOR<STRING> &arguments) override {
        if (arguments.empty()) {
            Error("Specify the type of assembly.");
            return;
        }

        auto type = arguments[0];
        if (!State::sys_build) {
            auto compiler_inf = State::compiler_inf;
            if (!compiler_inf) {
                compiler_inf = MakeGlobalPtr<CompilerInf>();
                compiler_inf->archiver = "ar.exe";
                compiler_inf->cxx_compiler = "g++.exe";
                compiler_inf->cxx_linker = "g++.exe";
                State::compiler_inf  = compiler_inf;
            }
            State::sys_build = MakeGlobalPtr<SysBuild>(compiler_inf.get());
            State::projectName = type;
        }
    }
};

#endif //FASTENGINE_BUILD_H
