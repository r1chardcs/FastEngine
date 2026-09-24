#ifndef FASTENGINE_TUIAPPLICATION_H
#define FASTENGINE_TUIAPPLICATION_H

#include "Platform.h"
namespace toolkit {
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
            BOOL HasFlag(const STRING& name) const;
            BOOL Has(const STRING& name) const;
            STRING Get(const STRING& name, const STRING& fallback = "") const;
            INT GetInt(const STRING& name, INT fallback = 0) const;
            FLOAT GetFloat(const STRING& name, FLOAT fallback = 0.0f) const;
            const VECTOR<STRING>& Positionals() const;
            STRING Positional(const SIZE_T index, const STRING& fallback = "") const;
        };

    private:
        RawBuff raw_buf;
        VECTOR<Argument> specs;

        const Argument* FindSpec(const STRING& token) const;

        ListArgument ParseArgs() const;
        void PrintUsageAndExit() const;
        static TuiApplication* instance;

    protected:
        VIRTUAL VOID RegisterArguments() {}

        void AddArgument(Argument arg);

    public:
        explicit TuiApplication(MOVE_PLEASE RawBuff raw_buff);
        VIRTUAL ~TuiApplication() = default;

        STATUS Run();

        static STRING GetCWD();

        VIRTUAL STATUS Main(const ListArgument& arguments, const STRING &cwd) = 0;

        DEPRECTED_API
        static STATUS Ok() { return 0; }

        DEPRECTED_API
        static STATUS Err() { return -1; }

        static VIEW_PTR<TuiApplication> GetInstance();
    };
}
#endif //FASTENGINE_TUIAPPLICATION_H