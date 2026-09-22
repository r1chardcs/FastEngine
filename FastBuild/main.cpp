#include <iostream>
#include <Toolkit/TUI/TuiApplication.h>
#include "SysBuild.h"
#include "Debug/SelfCrashDumper.h"
#include "Debug/Test.h"
#include "funcs/Build.h"
#include "funcs/CXX.h"
#include "funcs/Print.h"
#include "funcs/Target.h"
#include "lang/FastBuildLang.h"
#include "Toolkit/IO/IO.h"

class App : public TuiApplication {
public:
    App(const RawBuff &raw_buff)
        : TuiApplication(raw_buff) {
    }
protected:
    void RegisterArguments() override {
        AddArgument({"-file", {"-f"}, ArgType::Flag, false, "build.fb", "the main build file"});
        AddArgument({"-new", {"-n"}, ArgType::Flag, false, "", "create new project"});
        AddArgument({"-cmake", {"-c"}, ArgType::Flag, false, "", "migrate cmake file"});
    }
public:
    STATUS Main(const ListArgument &arguments, const STRING &cwd) override {
        if (const auto newproject = arguments.HasFlag("-new")) {
            IO::File::WriteFile("build.fb", R"(
build("my-game-fastengine")

target("app_game")
source += app.cpp

type(fastengine)

)");
            IO::File::WriteFile("app.cpp", R"(
#include <Engine/App.h>

class Game : public App {
public:
    Game() : App("Game") {}

    void Start() override {
        LOGINF.Output("My game start!").Output("\n");
    }
};

int main() {
    Game game;
    return game.Run();
}

)");

            return Ok();
        }
        const auto file = arguments.Get("-file", "build.fb");
        if (const auto [res, err] = IO::File::ExistFile(file); !res || err) {
            if (err) {
                LOGERR.Output("Error: %s\n", err);
                return Err();
            }
            LOGERR.Output("File not found: %s\n", file.c_str());
            return Err();
        }
        const auto [res, err] = IO::File::ReadFile(file);
        if (err) {
            LOGERR.Output("Error: %s\n", err);
            return Err();
        }
        FastBuildLang lang(res);

        lang.AddRuntimeFunc(MakeGlobalPtr<Build>());
        lang.AddRuntimeFunc(MakeGlobalPtr<Compiler>());
        lang.AddRuntimeFunc(MakeGlobalPtr<Linker>());
        lang.AddRuntimeFunc(MakeGlobalPtr<Archiver>());
        lang.AddRuntimeFunc(MakeGlobalPtr<Print>());
        lang.AddRuntimeFunc(MakeGlobalPtr<Toolchain>());
        lang.AddRuntimeFunc(MakeGlobalPtr<Jobs>());
        lang.AddRuntimeFunc(MakeGlobalPtr<Target>());
        lang.AddRuntimeFunc(MakeGlobalPtr<TargetType>());
        lang.AddRuntimeFunc(MakeGlobalPtr<TargetCompileFlag>());
        lang.AddRuntimeFunc(MakeGlobalPtr<TargetInclude>());
        lang.AddRuntimeFunc(MakeGlobalPtr<TargetSource>());
        lang.AddRuntimeFunc(MakeGlobalPtr<TargetLibraryDir>());
        lang.AddRuntimeFunc(MakeGlobalPtr<TargetDefine>());
        lang.AddRuntimeFunc(MakeGlobalPtr<TargetLinkFlag>());
        lang.Run();
        printf("%s\n", State::compiler_inf->cxx_compiler.c_str());

        if (State::sys_build) {
            for (const auto &target : State::all_targets) {
                TEST(not target);
                State::sys_build->AddTarget(*target);
            }
            if (arguments.HasFlag("-cmake")) {
                if (State::sys_build) {
                    const auto cmake = State::sys_build->MigrateToCmake();
                    LOGINF.Output(cmake.c_str()).Output("\n");
                    return Ok();
                }
            }
            if (const auto result = State::sys_build->BuildAll().err) {
                printf("%s\n", result);
                const auto last = State::sys_build->GetLastError();
                printf("%s\n", last.c_str());
            }
        }
        return Ok();
    }
};


int main(const int c, char** r) {
    App app(App::RawBuff(r, c));
    return app.Run();
}