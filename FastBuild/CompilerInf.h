#ifndef FASTENGINE_COMPILERINF_H
#define FASTENGINE_COMPILERINF_H
#include "Platform.h"

struct CompilerInf {
    STRING cxx_compiler;
    STRING cxx_linker;
    STRING archiver; 

    static CompilerInf FromToolchain(const STRING &path) {
        return {
            .cxx_compiler = path + "/g++.exe",
            .cxx_linker = path + "/g++.exe",
            .archiver = path + "/ar.exe"
        };
    }
};

#endif //FASTENGINE_COMPILERINF_H