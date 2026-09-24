//
// Created by dlllibstdntc on 12.09.2026.
//

#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>

void toolkit::Logger::SetOutputCallback(const CALLBACK<VOID(VIEW_PTR<Logger>, STRING)> &callback) {
    this->output_callback = callback;
}

toolkit::Logger & toolkit::Logger::Output(LITERAL format, ...) {
    va_list x;
    va_start(x, format);
    if (!output_callback) {
        vprintf_s(format, x);
    }
    else {
        va_list argsCopy;
        va_copy(argsCopy, x);
        const INT needed = _vscprintf(format, argsCopy);
        va_end(argsCopy);

        if (needed < 0) {
            return *this;
        }

        VECTOR<CHAR> buf(needed + 1);
        vsprintf_s(buf.data(), buf.size(), format, x);

        const STRING copy(buf.data(), needed);
        output_callback.operator()(this, copy);
    }
    va_end(x);
    return *this;
}

STRING toolkit::Logger::GetPrefix() {
    return prefix;
}

toolkit::Logger toolkit::LoggerFactory::New(STRING prefix) {
    return Logger(MOVE(prefix));
}

toolkit::Logger & toolkit::LoggerFactory::Error() {
    static Logger buf = New("ERROR");
    return buf;
}

toolkit::Logger & toolkit::LoggerFactory::Info() {
    static Logger buf = New("INFO");
    return buf;
}

toolkit::Logger & toolkit::LoggerFactory::Warn() {
    static Logger buf = New("WARN");
    return buf;
}
