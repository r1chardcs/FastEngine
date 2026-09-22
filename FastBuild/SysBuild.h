#ifndef FASTENGINE_SYSBUILD_H
#define FASTENGINE_SYSBUILD_H

#include <Platform.h>
#include <mutex>
#include <atomic>
#include <filesystem>

#include "CompilerInf.h"
#include "BuildTarget.h"

class SysBuild;

enum class BuildActionType {
    POST, PRE, INIT
};

struct BuildAction {
    VIRTUAL BuildActionType GetType() const noexcept = 0;
    VIRTUAL  void Action(VIEW_PTR<SysBuild> sys_build, VIEW_PTR<BuildTarget> build_target) = 0;
    VIRTUAL ~BuildAction() = default;
};

class SysBuild {
    VIEW_PTR<CompilerInf> compiler_inf;
    VECTOR<BuildTarget> targets;
    VECTOR<SELF_PTR<BuildAction>> actions;
    mutable STRING last_error;
    UINT max_parallel_jobs;

    struct CompileJob {
        STRING source;
        STRING object;
        STRING depfile;
    };

    struct CompileResult {
        BOOL success;
        STRING source;
        STRING log;
    };

    static std::filesystem::file_time_type GetFileMTime(const STRING& path);
    static BOOL FileExists(const STRING& path);
    static VECTOR<STRING> ParseDependencyFile(const STRING& depPath);
    static STRING ObjectPathFor(const BuildTarget& target, const STRING& source);
    static STRING PathToUnderscored(const STRING& path);

    BOOL NeedsRecompile(const STRING& source, const STRING& object, const STRING& depfile) const;
    STRING BuildCompileCommand(const BuildTarget& target, const CompileJob& job) const;
    STRING BuildLinkCommand(const BuildTarget& target, const VECTOR<STRING>& objects) const;

    CompileResult RunCompile(const BuildTarget& target, const CompileJob& job) const;
    Err<NOT> RunLink(const BuildTarget& target, const VECTOR<STRING>& objects) const;

    static Err<NOT> RunProcessCapture(const STRING& command, STRING& outLog);

public:
    explicit SysBuild(VIEW_PTR<CompilerInf> compiler_inf, UINT max_parallel_jobs = 0);

    void AddAction(MOVE_PLEASE SELF_PTR<BuildAction> build_action);

    template <typename TemplateBuildAction>
    void AddAction() {
        static_assert(std::is_base_of<BuildAction, TemplateBuildAction>(),
            "TemplateBuildAction must be devire from BuildAction");

        const auto newAction = MakeSelfPtr<TemplateBuildAction>();
        actions.emplace_back(MOVE(newAction));
    }

    void AddTarget(BuildTarget target);

    Err<NOT> Build(const STRING& target_name);
    Err<NOT> BuildAll();

    const STRING& GetLastError() const { return last_error; }

    STRING MigrateToCmake();
};

#endif //FASTENGINE_SYSBUILD_H