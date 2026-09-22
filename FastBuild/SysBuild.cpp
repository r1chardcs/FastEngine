#include "SysBuild.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <vector>

#include "../Toolkit/Debug/Logger.h"
#include "../Toolkit/IO/IO.h"
#include "Debug/Test.h"

#if defined(_WIN32)
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #define POPEN popen
    #define PCLOSE pclose
#endif

namespace fs = std::filesystem;

SysBuild::SysBuild(const VIEW_PTR<CompilerInf> compiler_inf, UINT max_parallel_jobs)
    : compiler_inf(compiler_inf) {
    this->max_parallel_jobs = max_parallel_jobs > 0
        ? max_parallel_jobs
        : std::max(1u, std::thread::hardware_concurrency());
}

void SysBuild::AddAction(SELF_PTR<BuildAction> build_action) {
    TEST(!build_action);
    if (build_action) {
        actions.emplace_back(MOVE(build_action));
    }
}

void SysBuild::AddTarget(BuildTarget target) {
    targets.push_back(MOVE(target));
}

fs::file_time_type SysBuild::GetFileMTime(const STRING& path) {
    std::error_code ec;
    const auto time = fs::last_write_time(path, ec);
    if (ec) {
        printf("Error: %s\n", ec.message().c_str());
    }
    return time;
}

BOOL SysBuild::FileExists(const STRING& path) {
    std::error_code ec;
    return fs::exists(path, ec) && !ec;
}

STRING SysBuild::PathToUnderscored(const STRING& path) {
    STRING result = path;
    for (auto& c : result) {
        if (c == '/' || c == '\\' || c == ':') {
            c = '_';
        }
    }
    return result;
}

STRING SysBuild::ObjectPathFor(const BuildTarget& target, const STRING& source) {
    const fs::path srcPath(source);
    STRING flatName = PathToUnderscored(srcPath.string());

    if (const auto dot = flatName.find_last_of('.'); dot != STRING::npos) {
        flatName = flatName.substr(0, dot);
    }
    flatName += ".o";

    const fs::path objDir(target.obj_dir);
    return (objDir / flatName).string();
}

VECTOR<STRING> SysBuild::ParseDependencyFile(const STRING& depPath) {
    VECTOR<STRING> deps;

    std::ifstream file(depPath);
    if (!file.is_open()) {
        return deps;
    }

    STRING content((std::istreambuf_iterator<CHAR>(file)), std::istreambuf_iterator<CHAR>());

    STRING flattened;
    flattened.reserve(content.size());
    for (SIZE_T i = 0; i < content.size(); i++) {
        if (content[i] == '\\' && i + 1 < content.size() &&
            (content[i + 1] == '\n' || content[i + 1] == '\r')) {
            flattened += ' ';
            i++;
            if (i + 1 < content.size() && content[i] == '\r' && content[i + 1] == '\n') {
                i++;
            }
            continue;
        }
        flattened += content[i];
    }

    std::istringstream stream(flattened);
    STRING token;
    BOOL first = true;

    while (stream >> token) {
        if (first) {
            first = false;
            continue;
        }
        deps.push_back(token);
    }

    return deps;
}

BOOL SysBuild::NeedsRecompile(const STRING& source, const STRING& object, const STRING& depfile) const {
    if (!FileExists(object)) {
        return true;
    }

    const auto objectTime = GetFileMTime(object);

    const auto sourceTime = GetFileMTime(source);
    if (sourceTime > objectTime) {
        return true;
    }

    if (!FileExists(depfile)) {
        return true;
    }

    const auto deps = ParseDependencyFile(depfile);
    for (const auto& dep : deps) {
        if (!FileExists(dep)) {
            return true;
        }
        const auto depTime = GetFileMTime(dep);
        if (depTime > objectTime) {
            return true;
        }
    }

    return false;
}
STRING SysBuild::BuildCompileCommand(const BuildTarget& target, const CompileJob& job) const {
    std::ostringstream cmd;

    cmd << "\"" << compiler_inf->cxx_compiler << "\""
        << " -std=" << target.cpp_standard
        << (target.optimize ? " -O2" : " -O0")
        << (target.debug_symbols ? " -g" : "")
        << " -MMD -MP"
        << " -c \"" << job.source << "\""
        << " -o \"" << job.object << "\"";

    for (const auto& inc : target.include_dirs) {
        cmd << " -I\"" << inc << "\"";
    }
    for (const auto& def : target.defines) {
        cmd << " -D" << def;
    }
    for (const auto& flag : target.extra_compile_flags) {
        cmd << " " << flag;
    }

    if (target.output_type == BuildOutputType::SharedLibrary) {
        cmd << " -fPIC";
    }

    return cmd.str();
}

STRING SysBuild::BuildLinkCommand(
    const BuildTarget& target,
    const VECTOR<STRING>& objects
) const {
    fs::path outDir(target.output_dir);

    STRING outputName = target.output_name;

    if (outputName.empty()) {
        outputName = target.name;
    }

    switch (target.output_type) {
        case BuildOutputType::Executable:
            if (!outputName.ends_with(".exe")) {
                outputName += ".exe";
            }
            break;

        case BuildOutputType::StaticLibrary:
            if (!outputName.ends_with(".a")) {
                outputName = "lib" + outputName + ".a";
            }
            break;

        case BuildOutputType::SharedLibrary:
            if (!outputName.ends_with(".dll")) {
                outputName += ".dll";
            }
            break;
    }

    const fs::path outPath = outDir / outputName;

    std::ostringstream cmd;

    if (target.output_type == BuildOutputType::StaticLibrary) {
        cmd << "\""
            << compiler_inf->archiver
            << "\" rcs \""
            << outPath.string()
            << "\"";

        for (const auto& obj : objects) {
            cmd << " \"" << obj << "\"";
        }

        return cmd.str();
    }

    cmd << "\""
        << compiler_inf->cxx_linker
        << "\"";

    for (const auto& obj : objects) {
        cmd << " \"" << obj << "\"";
    }

    if (target.output_type == BuildOutputType::SharedLibrary) {
        cmd << " -shared";
    }

    cmd << " -o \""
        << outPath.string()
        << "\"";

    for (const auto& libDir : target.library_dirs) {
        cmd << " -L\"" << libDir << "\"";
    }

    for (const auto& lib : target.libraries) {
        const bool isPath =
            lib.size() > 2 &&
            (
                lib.ends_with(".a") ||
                lib.ends_with(".o") ||
                lib.ends_with(".dll") ||
                lib.ends_with(".dll.a") ||
                lib.ends_with(".lib") ||
                lib.ends_with(".so")
            );

        if (isPath) {
            cmd << " \"" << lib << "\"";
        } else {
            cmd << " -l" << lib;
        }
    }

    for (const auto& flag : target.extra_link_flags) {
        cmd << " " << flag;
    }

    return cmd.str();
}

Err<NOT> SysBuild::RunProcessCapture(const STRING& command, STRING& outLog) {
    STRING fullCommand = command + " 2>&1";

#if defined(_WIN32)
    if (!fullCommand.empty() && fullCommand.front() == '"') {
        fullCommand = "\"" + fullCommand + "\"";
    }
#endif

    FILE* pipe = POPEN(fullCommand.c_str(), "r");
    if (!pipe) {
        return {.res = {}, .err = "Failed to start compiler process"};
    }

    CHAR buffer[512];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        outLog += buffer;
    }

    const INT exitCode = PCLOSE(pipe);

    if (exitCode != 0) {
        return {.res = {}, .err = "Process exited with error"};
    }

    return {.res = {}, .err = nullptr};
}

SysBuild::CompileResult SysBuild::RunCompile(const BuildTarget& target, const CompileJob& job) const {
    CompileResult result;
    result.source = job.source;

    const STRING command = BuildCompileCommand(target, job);
    LOGWRN.Output("Running: %s\n", command.c_str());

    const auto [res, err] = RunProcessCapture(command, result.log);

    result.success = (err == nullptr);
    return result;
}

Err<NOT> SysBuild::RunLink(const BuildTarget& target, const VECTOR<STRING>& objects) const {
    const STRING command = BuildLinkCommand(target, objects);
    STRING log;
    auto result = RunProcessCapture(command, log);

    if (result.err) {
        last_error = "Link failed:\n" + log;
        return {.res = {}, .err = "Link failed"};
    }

    return {.res = {}, .err = nullptr};
}

Err<NOT> SysBuild::Build(const STRING& target_name) {
    BuildTarget* targetPtr = nullptr;
    for (auto& t : targets) {
        if (t.name == target_name) {
            targetPtr = &t;
            break;
        }
    }

    if (!targetPtr) {
        last_error = "Target not found: " + target_name;
        return {.res = {}, .err = "Target not found"};
    }

    BuildTarget& target = *targetPtr;

    for (const auto &build_action : actions) {
        if (build_action->GetType() == BuildActionType::PRE)
            build_action->Action(this, &target);
    }

    std::error_code ec;
    fs::create_directories(target.obj_dir, ec);
    fs::create_directories(target.output_dir, ec);

    VECTOR<CompileJob> jobs;
    VECTOR<STRING> allObjects;

    for (const auto& source : target.sources) {
        const STRING object = ObjectPathFor(target, source);
        const STRING depfile = object.substr(0, object.find_last_of('.')) + ".d";

        allObjects.push_back(object);

        if (NeedsRecompile(source, object, depfile)) {
            jobs.push_back({source, object, depfile});
        }
    }

    if (jobs.empty()) {
        LOGWRN.Output("Target '%s' is up to date, nothing to compile\n", target.name.c_str());
    } else {
        LOGWRN.Output("Compiling %zu of %zu source(s) for target '%s'\n",
                       jobs.size(), target.sources.size(), target.name.c_str());

        std::mutex queueMutex;
        std::queue<SIZE_T> jobQueue;
        for (SIZE_T i = 0; i < jobs.size(); i++) {
            jobQueue.push(i);
        }

        std::mutex resultsMutex;
        VECTOR<CompileResult> results(jobs.size());
        std::atomic<BOOL> hadError{false};

        const UINT workerCount = std::min(max_parallel_jobs, static_cast<UINT>(jobs.size()));
        VECTOR<std::thread> workers;
        workers.reserve(workerCount);

        for (UINT w = 0; w < workerCount; w++) {
            workers.emplace_back([&]() {
                while (true) {
                    SIZE_T jobIndex;
                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        if (jobQueue.empty()) return;
                        jobIndex = jobQueue.front();
                        jobQueue.pop();
                    }

                    const auto result = RunCompile(target, jobs[jobIndex]);

                    {
                        std::lock_guard<std::mutex> lock(resultsMutex);
                        results[jobIndex] = result;
                        if (!result.success) {
                            hadError.store(true);
                        }
                    }
                }
            });
        }

        for (auto& worker : workers) {
            worker.join();
        }

        for (const auto& result : results) {
            if (!result.log.empty()) {
                LOGWRN.Output("%s:\n%s\n", result.source.c_str(), result.log.c_str());
            }
        }

        if (hadError.load()) {
            last_error = "One or more source files failed to compile for target: " + target.name;
            return {.res = {}, .err = "Compilation failed"};
        }
    }

    LOGWRN.Output("Linking target '%s'...\n", target.name.c_str());
    const auto result = RunLink(target, allObjects);
    for (const auto &build_action : actions) {
        if (build_action->GetType() == BuildActionType::POST)
            build_action->Action(this, &target);
    }
    return result;
}

Err<NOT> SysBuild::BuildAll() {
    for (auto& target : targets) {
        for (const auto &build_action : actions) {
            if (build_action->GetType() == BuildActionType::INIT)
                build_action->Action(this, &target);
        }
        if (const auto result = Build(target.name); result.err) {
            return result;
        }
    }
    return {.res = {}, .err = nullptr};
}

STRING SysBuild::MigrateToCmake() {
    std::ostringstream cmake;

    cmake << "cmake_minimum_required(VERSION 3.20)\n\n";
    cmake << "project(FastBuildMigration LANGUAGES CXX)\n\n";

    cmake << "set(CMAKE_CXX_STANDARD 20)\n";
    cmake << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n";
    cmake << "set(CMAKE_CXX_EXTENSIONS OFF)\n\n";

    for (auto& target : targets) {
        if (target.name.empty()) {
            continue;
        }

        cmake << "# ============================================================\n";
        cmake << "# Target: " << target.name << "\n";
        cmake << "# ============================================================\n\n";

        switch (target.output_type) {
            case BuildOutputType::Executable:
                cmake << "add_executable(" << target.name << "\n";
                break;

            case BuildOutputType::StaticLibrary:
                cmake << "add_library(" << target.name << " STATIC\n";
                break;

            case BuildOutputType::SharedLibrary:
                cmake << "add_library(" << target.name << " SHARED\n";
                break;
        }

        for (const auto& source : target.sources) {
            cmake << "    \"" << source << "\"\n";
        }

        cmake << ")\n\n";

        if (!target.include_dirs.empty()) {
            cmake << "target_include_directories(" << target.name << " PUBLIC\n";

            for (auto& include : target.include_dirs) {
                std::ranges::replace(include, '\\', '/');
                cmake << "    \"" << include << "\"\n";
            }

            cmake << ")\n\n";
        }

        if (!target.defines.empty()) {
            cmake << "target_compile_definitions(" << target.name << " PRIVATE\n";

            for (const auto& define : target.defines) {
                cmake << "    " << define << "\n";
            }

            cmake << ")\n\n";
        }

        if (!target.extra_compile_flags.empty()) {
            cmake << "target_compile_options(" << target.name << " PRIVATE\n";

            for (const auto& flag : target.extra_compile_flags) {
                cmake << "    " << flag << "\n";
            }

            cmake << ")\n\n";
        }

        if (!target.library_dirs.empty()) {
            cmake << "target_link_directories(" << target.name << " PRIVATE\n";

            for (const auto& dir : target.library_dirs) {
                cmake << "    \"" << dir << "\"\n";
            }

            cmake << ")\n\n";
        }

        if (!target.libraries.empty()) {
            cmake << "target_link_libraries(" << target.name << " PRIVATE\n";

            for (auto& library : target.libraries) {
                std::ranges::replace(library, '\\', '/');
                cmake << "    \"" << library << "\"\n";
            }

            cmake << ")\n\n";
        }

        if (!target.extra_link_flags.empty()) {
            cmake << "target_link_options(" << target.name << " PRIVATE\n";

            for (const auto& flag : target.extra_link_flags) {
                cmake << "    " << flag << "\n";
            }

            cmake << ")\n\n";
        }

        if (!target.cpp_standard.empty()) {
            cmake << "set_target_properties(" << target.name << " PROPERTIES\n";
            cmake << "    CXX_STANDARD " << target.cpp_standard.substr(5) << "\n";
            cmake << "    CXX_STANDARD_REQUIRED ON\n";
            cmake << "    CXX_EXTENSIONS OFF\n";
            cmake << ")\n\n";
        }

        if (!target.optimize) {
            cmake << "target_compile_options(" << target.name << " PRIVATE\n";
            cmake << "    $<$<CONFIG:Release>:-O0>\n";
            cmake << "    $<$<CONFIG:Debug>:-O0>\n";
            cmake << ")\n\n";
        } else {
            cmake << "target_compile_options(" << target.name << " PRIVATE\n";
            cmake << "    $<$<CONFIG:Release>:-O2>\n";
            cmake << ")\n\n";
        }

        if (target.debug_symbols) {
            cmake << "target_compile_options(" << target.name << " PRIVATE\n";
            cmake << "    $<$<CONFIG:Debug>:-g>\n";
            cmake << ")\n\n";
        }

        if (target.output_type == BuildOutputType::SharedLibrary) {
            cmake << "set_target_properties(" << target.name << " PROPERTIES\n";
            cmake << "    POSITION_INDEPENDENT_CODE ON\n";
            cmake << ")\n\n";
        }

        if (!target.output_name.empty() || target.output_dir != "build/bin") {
            cmake << "set_target_properties(" << target.name << " PROPERTIES\n";

            if (!target.output_name.empty()) {
                STRING outputName = target.output_name;

                if (target.output_type == BuildOutputType::Executable) {
                    if (outputName.ends_with(".exe")) {
                        outputName = outputName.substr(0, outputName.size() - 4);
                    }

                    cmake << "    OUTPUT_NAME \"" << outputName << "\"\n";
                } else if (target.output_type == BuildOutputType::StaticLibrary) {
                    if (outputName.starts_with("lib")) {
                        outputName = outputName.substr(3);
                    }

                    if (outputName.ends_with(".a")) {
                        outputName = outputName.substr(0, outputName.size() - 2);
                    }

                    cmake << "    OUTPUT_NAME \"" << outputName << "\"\n";
                } else if (target.output_type == BuildOutputType::SharedLibrary) {
                    if (outputName.ends_with(".dll")) {
                        outputName = outputName.substr(0, outputName.size() - 4);
                    }

                    cmake << "    OUTPUT_NAME \"" << outputName << "\"\n";
                }
            }

            if (target.output_dir != "build/bin") {
                cmake << "    RUNTIME_OUTPUT_DIRECTORY \""
                      << target.output_dir << "\"\n";

                cmake << "    LIBRARY_OUTPUT_DIRECTORY \""
                      << target.output_dir << "\"\n";

                cmake << "    ARCHIVE_OUTPUT_DIRECTORY \""
                      << target.output_dir << "\"\n";
            }

            cmake << ")\n\n";
        }
    }

    const STRING content = cmake.str();

    std::ofstream file("CMakeLists.txt", std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        last_error = "Failed to create CMakeLists.txt";
        return {};
    }

    file << content;
    file.close();

    return content;
}