//
// Created by dlllibstdntc on 21.09.2026.
//

#ifndef FASTENGINE_COMPILER_H
#define FASTENGINE_COMPILER_H

#include <complex>

#include  "../lang/RuntimeFunc.h"
#include "FastBuild/State.h"

class Compiler : public RuntimeFunc {
public:
    Compiler()
        : RuntimeFunc("cxx-compiler") {
    }

    void Run(const VECTOR<STRING> &arguments) override {
        if (arguments.empty()) {
            Error("Specify the type of assembly.");
            return;
        }

        auto compiler_inf = State::compiler_inf;
        if (!compiler_inf) {
            compiler_inf = MakeGlobalPtr<CompilerInf>();
            State::compiler_inf = compiler_inf;
        }

        compiler_inf->cxx_compiler = arguments[0];
    }
};

class Linker : public RuntimeFunc {
public:
    Linker()
        : RuntimeFunc("cxx-linker") {
    }

    void Run(const VECTOR<STRING> &arguments) override {
        if (arguments.empty()) {
            Error("Specify the type of assembly.");
            return;
        }

        auto compiler_inf = State::compiler_inf;
        if (!compiler_inf) {
            compiler_inf = MakeGlobalPtr<CompilerInf>();
            State::compiler_inf = compiler_inf;
        }

        compiler_inf->cxx_linker = arguments[0];
    }
};

class Archiver : public RuntimeFunc {
public:
    Archiver()
        : RuntimeFunc("cxx-ar") {
    }

    void Run(const VECTOR<STRING> &arguments) override {
        if (arguments.empty()) {
            Error("Specify the type of assembly.");
            return;
        }

        auto compiler_inf = State::compiler_inf;
        if (!compiler_inf) {
            compiler_inf = MakeGlobalPtr<CompilerInf>();
            State::compiler_inf = compiler_inf;
        }

        compiler_inf->archiver = arguments[0];
    }
};

class Jobs : public RuntimeFunc {
public:
    Jobs()
        : RuntimeFunc("cxx-jobs") {
    }

    void Run(const VECTOR<STRING> &arguments) override {
        if (arguments.empty()) {
            Error("Specify the type of assembly.");
            return;
        }

        State::jobs = atoi(arguments[0].c_str());
    }
};

class Toolchain : public RuntimeFunc {
public:
    Toolchain() : RuntimeFunc("toolchain") {

    }

    void Run(const VECTOR<STRING> &arguments) override {
        if (arguments.empty()) {
            Error("Specify the type of assembly.");
            return;
        }


        auto compiler_inf = State::compiler_inf;
        if (!compiler_inf) {
            compiler_inf = MakeGlobalPtr<CompilerInf>();
            State::compiler_inf = compiler_inf;
        }

        auto [cxx_compiler,
             cxx_linker,
             archiver] = CompilerInf::FromToolchain(arguments[0]);

        compiler_inf->cxx_compiler = cxx_compiler;
        compiler_inf->cxx_linker = cxx_linker;
        compiler_inf->archiver = archiver;
    }
};

#endif //FASTENGINE_COMPILER_H
