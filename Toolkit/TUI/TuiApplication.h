#ifndef FASTENGINE_TUIAPPLICATION_H
#define FASTENGINE_TUIAPPLICATION_H

#include "Platform.h"

class TuiApplication {
public:
    struct RawBuff { CHAR** r; INT c; };

    enum class ArgType {
        Flag,
        Option,
        Positional
    };

    struct Argument {
        STRING name;
        VECTOR<STRING> aliases;
        ArgType type = ArgType::Option;
        BOOL required = false;
        STRING defaultValue;
        STRING description;
    };

    class ListArgument {
        HASH_MAP<STRING, STRING> values;
        HASH_MAP<STRING, BOOL> flags;
        VECTOR<STRING> positionals;

        friend class TuiApplication;

    public:
        BOOL HasFlag(const STRING& name) const {
            const auto it = flags.find(name);
            return it != flags.end() && it->second;
        }

        BOOL Has(const STRING& name) const {
            return values.find(name) != values.end();
        }

        STRING Get(const STRING& name, const STRING& fallback = "") const {
            const auto it = values.find(name);
            return it != values.end() ? it->second : fallback;
        }

        INT GetInt(const STRING& name, INT fallback = 0) const {
            const auto it = values.find(name);
            if (it == values.end()) return fallback;
            try { return std::stoi(it->second); }
            catch (...) { return fallback; }
        }

        FLOAT GetFloat(const STRING& name, FLOAT fallback = 0.0f) const {
            const auto it = values.find(name);
            if (it == values.end()) return fallback;
            try { return std::stof(it->second); }
            catch (...) { return fallback; }
        }

        const VECTOR<STRING>& Positionals() const {
            return positionals;
        }

        STRING Positional(SIZE_T index, const STRING& fallback = "") const {
            return index < positionals.size() ? positionals[index] : fallback;
        }
    };

private:
    RawBuff raw_buf;
    VECTOR<Argument> specs;

    const Argument* FindSpec(const STRING& token) const;

    ListArgument ParseArgs() const;
    void PrintUsageAndExit() const;

protected:
    VIRTUAL VOID RegisterArguments() {}

    void AddArgument(Argument arg);

public:
    explicit TuiApplication(MOVE_PLEASE RawBuff raw_buff);
    VIRTUAL ~TuiApplication() = default;

    STATUS Run();

    VIRTUAL STATUS Main(const ListArgument& arguments, const STRING &cwd) = 0;

    static STATUS Ok() { return 0; }
    static STATUS Err() { return -1; }
};

#endif //FASTENGINE_TUIAPPLICATION_H