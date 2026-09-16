//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_IO_H
#define FASTENGINE_IO_H
#include <Platform.h>

namespace IO {
    STRING InputString(const STRING & = "");
    CHAR InputChar(const STRING & = "");

    namespace File {
        Err<STRING> ReadFile(const STRING& path);
        Err<VECTOR<BYTE>> ReadFileBytes(const STRING& path);

        Err<BOOL> ExistFile(const STRING& path);
        Err<BOOL> ExistDirectory(const STRING& path);

        Err<NOT> WriteFile(const STRING& path);
        Err<NOT> CreateDirectory(const STRING& path);
    }
};


#endif //FASTENGINE_IO_H
