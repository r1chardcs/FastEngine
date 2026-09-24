#include "TuiApplication.h"

#include <iostream>
#include <filesystem>
namespace toolkit {
    TuiApplication* TuiApplication::instance;

    TuiApplication::TuiApplication(RawBuff raw_buff) : raw_buf(MOVE(raw_buff)) {
        instance = this;
    }

    void TuiApplication::AddArgument(Argument arg) {
        specs.push_back(MOVE(arg));
    }

    BOOL TuiApplication::ListArgument::HasFlag(const STRING &name) const {
        const auto it = flags.find(name);
        return it != flags.end() && it->second;
    }

    BOOL TuiApplication::ListArgument::Has(const STRING &name) const {
        return values.find(name) != values.end();
    }

    STRING TuiApplication::ListArgument::Get(const STRING &name, const STRING &fallback) const {
        const auto it = values.find(name);
        return it != values.end() ? it->second : fallback;
    }

    INT TuiApplication::ListArgument::GetInt(const STRING &name, const INT fallback) const {
        const auto it = values.find(name);
        if (it == values.end()) return fallback;
        try { return std::stoi(it->second); }
        catch (...) { return fallback; }
    }

    FLOAT TuiApplication::ListArgument::GetFloat(const STRING &name, FLOAT fallback) const {
        const auto it = values.find(name);
        if (it == values.end()) return fallback;
        try { return std::stof(it->second); }
        catch (...) { return fallback; }
    }

    const VECTOR<STRING> & TuiApplication::ListArgument::Positionals() const {
        return positionals;
    }

    STRING TuiApplication::ListArgument::Positional(const SIZE_T index, const STRING &fallback) const {
        return index < positionals.size() ? positionals[index] : fallback;
    }

    const TuiApplication::Argument* TuiApplication::FindSpec(const STRING& token) const {
        for (const auto& spec : specs) {
            if (spec.name == token) return &spec;
            for (const auto& alias : spec.aliases) {
                if (alias == token) return &spec;
            }
        }
        return nullptr;
    }

    void TuiApplication::PrintUsageAndExit() const {
        std::cout << "Usage:\n";
        for (const auto& spec : specs) {
            std::cout << "  ";
            for (SIZE_T i = 0; i < spec.aliases.size(); i++) {
                std::cout << spec.aliases[i];
                if (i + 1 < spec.aliases.size()) std::cout << ", ";
            }
            if (spec.aliases.empty()) std::cout << spec.name;

            std::cout << "\t" << spec.description;
            if (spec.required) std::cout << " (required)";
            std::cout << "\n";
        }
        std::exit(1);
    }

    TuiApplication::ListArgument TuiApplication::ParseArgs() const {
        ListArgument result;

        for (const auto& spec : specs) {
            if (spec.type == ArgType::Option && !spec.defaultValue.empty()) {
                result.values[spec.name] = spec.defaultValue;
            }
            if (spec.type == ArgType::Flag) {
                result.flags[spec.name] = false;
            }
        }

        VECTOR<BOOL> seen(specs.size(), false);

        for (INT i = 1; i < raw_buf.c; i++) {
            const STRING token = raw_buf.r[i];

            const Argument* spec = FindSpec(token);

            if (!spec) {
                result.positionals.push_back(token);
                continue;
            }

            const SIZE_T specIndex = spec - specs.data();
            if (specIndex < seen.size()) seen[specIndex] = true;

            switch (spec->type) {
                case ArgType::Flag:
                    result.flags[spec->name] = true;
                    break;

                case ArgType::Option:
                    if (i + 1 >= raw_buf.c) {
                        std::cerr << "Missing value for argument: " << token << "\n";
                        PrintUsageAndExit();
                    }
                    result.values[spec->name] = raw_buf.r[++i];
                    break;

                case ArgType::Positional:
                    result.values[spec->name] = token;
                    result.positionals.push_back(token);
                    break;
            }
        }

        for (SIZE_T i = 0; i < specs.size(); i++) {
            const auto& spec = specs[i];
            if (spec.required && !seen[i] && spec.defaultValue.empty()) {
                std::cerr << "Missing required argument: " << spec.name << "\n";
                const_cast<TuiApplication*>(this)->PrintUsageAndExit();
            }
        }

        return result;
    }

    STATUS TuiApplication::Run() {
        RegisterArguments();

        const ListArgument arguments = ParseArgs();
        const STRING cwd = std::filesystem::current_path().string();

        return Main(arguments, cwd);
    }

    STRING TuiApplication::GetCWD() {
        return std::filesystem::current_path().string();
    }

    VIEW_PTR<TuiApplication> TuiApplication::GetInstance() {
        return instance;
    }
}