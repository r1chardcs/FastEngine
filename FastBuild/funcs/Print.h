//
// Created by dlllibstdntc on 21.09.2026.
//

#ifndef FASTENGINE_PRINT_H
#define FASTENGINE_PRINT_H

#include <iostream>

#include  "../lang/RuntimeFunc.h"

class Print : public RuntimeFunc {
public:
    Print()
        : RuntimeFunc("print") {
    }

    void Run(const VECTOR<STRING> &arguments) override {
        for (SIZE_T i = 0; i < arguments.size(); i++) {
            std::cout << arguments[i];
            if (i + 1 < arguments.size()) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
};

#endif //FASTENGINE_PRINT_H
