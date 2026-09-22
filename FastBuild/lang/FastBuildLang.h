//
// Created by dlllibstdntc on 21.09.2026.
//

#ifndef FASTENGINE_FASTBUILDLANG_H
#define FASTENGINE_FASTBUILDLANG_H

#include "DefFunc.h"
#include "RuntimeFunc.h"

class FastBuildLang {
    VECTOR<DefFunc> funcs;
    HASH_MAP<STRING, GLOBAL_PTR<RuntimeFunc>> runtime_funcs;
    STRING sources;

    void CollectDefFunc();
public:
    FastBuildLang(const STRING &sources) : sources(sources) {
        CollectDefFunc();
    }

    void AddRuntimeFunc(GLOBAL_PTR<RuntimeFunc> func);
    void Run();

    VECTOR<DefFunc> Funcs() {
        return funcs;
    }
};


#endif //FASTENGINE_FASTBUILDLANG_H
