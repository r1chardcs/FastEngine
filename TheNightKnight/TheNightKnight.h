//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_THENIGHTKNIGHT_H
#define FASTENGINE_THENIGHTKNIGHT_H

#include <Engine/App.h>
#include <Engine/objects/Rect.h>

class TheNightKnight : public App {
public:
    TheNightKnight() : App("The Night Knight") {}

    void Start() override {
        LOGWRN.Output("Coins: %d\n", local_storage->Get("coins", 0));
        local_storage->Set("coins", 10);
        local_storage->Save();
    }
};


#endif //FASTENGINE_THENIGHTKNIGHT_H
