//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_LOGGER_H
#define FASTENGINE_LOGGER_H

#include <Platform.h>

#undef CALLBACK

namespace toolkit {
    class Logger {
        CALLBACK<VOID(VIEW_PTR<Logger>, STRING)> output_callback = nullptr;
        STRING prefix;
    public:
        Logger(const STRING&& prefix) : prefix(MOVE(prefix)) {}
        Logger(const STRING& prefix) : prefix(prefix) {}

        void SetOutputCallback(const CALLBACK<void(VIEW_PTR<Logger>, STRING)> &callback);

        Logger& Output(LITERAL format, ...);
        STRING GetPrefix();
    };

    namespace LoggerFactory {
        Logger New(MOVE_PLEASE STRING prefix);

        Logger& Error();
        Logger& Info();
        Logger& Warn();
    };

    INLINE Logger LOGERR = LoggerFactory::Error();
    INLINE Logger LOGINF = LoggerFactory::Info();
    INLINE Logger LOGWRN = LoggerFactory::Warn();
}

#endif //FASTENGINE_LOGGER_H
