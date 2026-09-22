#include "BuildTarget.h"

#include <windows.h>

STRING GetExecutablePath() {
    CHAR buffer[MAX_PATH]{};

    const DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length == 0) {
        return {};
    }

    STRING path(buffer, length);

    const SIZE_T separator = path.find_last_of("\\/");
    if (separator == STRING::npos) {
        return {};
    }

    return path.substr(0, separator);   // <-- без +1
}