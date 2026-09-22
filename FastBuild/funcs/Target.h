//
// Created by dlllibstdntc on 21.09.2026.
//

#ifndef FASTENGINE_TARGET_H
#define FASTENGINE_TARGET_H
#include  "../lang/RuntimeFunc.h"
#include "FastBuild/State.h"

class Target : public RuntimeFunc {
public:
    Target()
        : RuntimeFunc("target") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        if (arguments.empty()) {
            Error("Specify target name.");
            return;
        }

        if (!State::sys_build) {
            Error("Build system is not initialized.");
            return;
        }

        const auto target = MakeGlobalPtr<BuildTarget>();

        target->name = arguments[0];

        State::all_targets.push_back(target);
        State::current_target = target;
    }
};

class TargetCommand : public RuntimeFunc {
protected:
    static BuildTarget* GetTarget() {
        if (!State::current_target) {
            Error("No target selected.");
            return nullptr;
        }

        return State::current_target.get();
    }

public:
    explicit TargetCommand(const STRING& name)
        : RuntimeFunc(name) {
    }
};

class TargetSource : public TargetCommand {
public:
    TargetSource()
        : TargetCommand("source") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        auto* target = GetTarget();

        if (!target) {
            return;
        }

        if (arguments.empty()) {
            Error("Specify source file.");
            return;
        }

        for (const auto& source : arguments) {
            target->sources.push_back(source);
        }
    }
};

class TargetInclude : public TargetCommand {
public:
    TargetInclude()
        : TargetCommand("include") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        auto* target = GetTarget();

        if (!target) {
            return;
        }

        if (arguments.empty()) {
            Error("Specify include directory.");
            return;
        }

        for (const auto& directory : arguments) {
            target->include_dirs.push_back(directory);
        }
    }
};

class TargetDefine : public TargetCommand {
public:
    TargetDefine()
        : TargetCommand("define") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        auto* target = GetTarget();

        if (!target) {
            return;
        }

        if (arguments.empty()) {
            Error("Specify preprocessor definition.");
            return;
        }

        for (const auto& define : arguments) {
            target->defines.push_back(define);
        }
    }
};

class TargetLibraryDir : public TargetCommand {
public:
    TargetLibraryDir()
        : TargetCommand("library-dir") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        auto* target = GetTarget();

        if (!target) {
            return;
        }

        if (arguments.empty()) {
            Error("Specify library directory.");
            return;
        }

        for (const auto& directory : arguments) {
            target->library_dirs.push_back(directory);
        }
    }
};

class TargetLibrary : public TargetCommand {
public:
    TargetLibrary()
        : TargetCommand("library") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        auto* target = GetTarget();

        if (!target) {
            return;
        }

        if (arguments.empty()) {
            Error("Specify library.");
            return;
        }

        for (const auto& library : arguments) {
            target->libraries.push_back(library);
        }
    }
};

class TargetCompileFlag : public TargetCommand {
public:
    TargetCompileFlag()
        : TargetCommand("compile-flag") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        auto* target = GetTarget();

        if (!target) {
            return;
        }

        if (arguments.empty()) {
            Error("Specify compile flag.");
            return;
        }

        for (const auto& flag : arguments) {
            target->extra_compile_flags.push_back(flag);
        }
    }
};

class TargetLinkFlag : public TargetCommand {
public:
    TargetLinkFlag()
        : TargetCommand("link-flag") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        auto* target = GetTarget();

        if (!target) {
            return;
        }

        if (arguments.empty()) {
            Error("Specify link flag.");
            return;
        }

        for (const auto& flag : arguments) {
            target->extra_link_flags.push_back(flag);
        }
    }
};

class TargetType : public TargetCommand {
public:
    TargetType()
        : TargetCommand("type") {
    }

    void Run(const VECTOR<STRING>& arguments) override {
        auto* target = GetTarget();

        if (!target) {
            return;
        }

        if (arguments.empty()) {
            Error("Specify target type.");
            return;
        }

        const auto& type = arguments[0];
        if (type == "fastengine"
            || type == "game") {
            target->output_type =
                BuildOutputType::Executable;

            const auto path = GetExecutablePath();
            target->include_dirs.push_back(path + "/core");
            target->include_dirs.push_back(path + "/core/include");
            target->include_dirs.push_back(path + "/core/include/include");
            target->libraries.push_back(path + "/core/libs/libFastEngine.dll.a");
        }
        else if (type == "executable" ||
            type == "exe") {
            target->output_type =
                BuildOutputType::Executable;
        }
        else if (type == "static" ||
                       type == "static-library" ||
                       type == "staticlib") {
            target->output_type =
            BuildOutputType::StaticLibrary;
        }
        else if (type == "shared" ||
                          type == "shared-library" ||
                          type == "sharedlib" ||
                          type == "dll") {
            target->output_type =
                      BuildOutputType::SharedLibrary;
        }
        else {
            Error("Unknown target.");
        }
    }
};

#endif //FASTENGINE_TARGET_H
