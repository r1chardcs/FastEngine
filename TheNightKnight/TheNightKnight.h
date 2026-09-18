//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_THENIGHTKNIGHT_H
#define FASTENGINE_THENIGHTKNIGHT_H

#include <sstream>
#include <Engine/App.h>
#include <Engine/Objects/Rect.h>

#include "scenes/MainScene.h"
#include "Toolkit/IO/IO.h"

class TheNightKnight : public App {
public:
    TheNightKnight() : App("The Night Knight") {}

    void Start() override {
        SetScene(MakeGlobalPtr<MainScene>());
    }

    void GrabSelfCrash(const CrashContext &ctx) override {
        App::GrabSelfCrash(ctx);
        std::ostringstream ss;
        std::ostringstream frameStream;
        frameStream << " at " << ctx.stack_trace[0].function_name;

        if (!ctx.stack_trace[0].module_name.empty()) {
            frameStream << " [" << ctx.stack_trace[0].module_name << "]";
        }

        if (!ctx.stack_trace[0].file_name.empty()) {
            frameStream << " (" << ctx.stack_trace[0].file_name << ":" << ctx.stack_trace[0].line_number << ")";
        }

        ss << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(ctx.exception_address);
        IO::MessageBox("Unhandled Exception", "Exception at 0x" + (ss.str()) + " Throw " + frameStream.str(), static_cast<MessageBoxFlags_t>(MessageBoxStyle::IconError | MessageBoxStyle::Ok));
    }
};


#endif //FASTENGINE_THENIGHTKNIGHT_H
