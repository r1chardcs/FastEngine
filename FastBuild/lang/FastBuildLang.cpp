



#include "FastBuildLang.h"

#include <cstring>

#include "Toolkit/Debug/Logger.h"

void FastBuildLang::CollectDefFunc() {
    SIZE_T i = 0;
    const SIZE_T len = sources.size();

    auto skipWhitespaceExceptNewline = [&]() {
        while (i < len && std::isspace(static_cast<unsigned char>(sources[i])) && sources[i] != '\n') {
            i++;
        }
    };

    auto skipWhitespace = [&]() {
        while (i < len && std::isspace(static_cast<unsigned char>(sources[i]))) {
            i++;
        }
    };

    auto readArgument = [&](DefFunc& func, auto isTerminator) -> BOOL {
        if (i < len && sources[i] == '"') {
            i++;
            const SIZE_T strStart = i;

            while (i < len && sources[i] != '"') {
                if (sources[i] == '\\' && i + 1 < len) {
                    i += 2;
                } else {
                    i++;
                }
            }

            func.arguments.push_back(sources.substr(strStart, i - strStart));

            if (i < len && sources[i] == '"') {
                i++;
            }
            return true;
        }

        const SIZE_T argStart = i;
        while (i < len && !isTerminator(i) && !std::isspace(static_cast<unsigned char>(sources[i]))) {
            i++;
        }

        if (i > argStart) {
            func.arguments.push_back(sources.substr(argStart, i - argStart));
            return true;
        }
        return false;
    };

    while (i < len) {
        skipWhitespace();
        if (i >= len) break;

        if (std::isalpha(static_cast<unsigned char>(sources[i])) || sources[i] == '_') {
            const SIZE_T nameStart = i;
            while (i < len && (std::isalnum(static_cast<unsigned char>(sources[i])) || sources[i] == '_')) {
                i++;
            }
            const STRING name = sources.substr(nameStart, i - nameStart);

            skipWhitespaceExceptNewline();

            if (i < len && sources[i] == '(') {
                i++;

                DefFunc func;
                func.name = name;

                skipWhitespace();

                while (i < len && sources[i] != ')') {
                    skipWhitespace();

                    readArgument(func, [&](SIZE_T pos) {
                        return sources[pos] == ',' || sources[pos] == ')';
                    });

                    skipWhitespace();

                    if (i < len && sources[i] == ',') {
                        i++;
                        skipWhitespace();
                    } else {
                        break;
                    }
                }

                if (i < len && sources[i] == ')') {
                    i++;
                }

                funcs.push_back(MOVE(func));
                continue;
            }

            if (i + 1 < len && sources[i] == '+' && sources[i + 1] == '=') {
                DefFunc func;
                func.name = name;

                while (i + 1 < len && sources[i] == '+' && sources[i + 1] == '=') {
                    i += 2;
                    skipWhitespaceExceptNewline();

                    readArgument(func, [&](SIZE_T pos) {
                        if (sources[pos] == '\n') return true;
                        if (pos + 1 < len && sources[pos] == '+' && sources[pos + 1] == '=') return true;
                        return false;
                    });

                    skipWhitespaceExceptNewline();
                }

                funcs.push_back(MOVE(func));
                continue;
            }
        }

        i++;
    }
}
void FastBuildLang::AddRuntimeFunc(GLOBAL_PTR<RuntimeFunc> func) {
    runtime_funcs[func->GetName()] = func;
}

void FastBuildLang::Run() {
    for (const auto&[name, arguments] : funcs) {
        const auto it = runtime_funcs.find(name);

        if (it == runtime_funcs.end()) {
            toolkit::LOGERR.Output("FastBuildLang: unknown function '%s'\n", name.c_str());
            continue;
        }

        it->second->Run(arguments);
    }
}
