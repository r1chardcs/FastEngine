//
// Created by dlllibstdntc on 17.09.2026.
//

#include <iostream>

#include "TheNightKnight.h"
void* operator new(const std::size_t size) {
    void* ptr = std::malloc(size);

    if (!ptr)
        throw std::bad_alloc();

    std::cout << "[Memory] Allocated " << size
              << " bytes at " << ptr << '\n';

    return ptr;
}
int main() {
    TheNightKnight game;
    return game.Run();
}