//
// Created by dlllibstdntc on 16.09.2026.
//

#ifndef FASTENGINE_ASSETS_H
#define FASTENGINE_ASSETS_H
#include "Platform.h"

DEPRECTED_API
struct AssetFile {
    STRING name;
    STRING path;
    STRING extension;

    VECTOR<BYTE> GetBytes();
    STRING GetString() const;
};

class Assets {
    STRING dir;
public:
    Assets(MOVE_PLEASE STRING dir);

    Err<AssetFile> GetFile(const STRING& path);

    template <typename... TArgs>
    STRING CombinePath(const STRING& path, TArgs... args);
};

template<typename ... TArgs>
STRING Assets::CombinePath(const STRING &path, TArgs... args) {
    STRING result = path;
    auto append = [&result](const auto& part) {
        if (result.empty()) {
            result = part;
            return;
        }
        if (result.back() != '/' && result.back() != '\\')
            result += '/';
        STRING p = part;
        while (!p.empty() && (p.front() == '/' || p.front() == '\\'))
            p.erase(p.begin());
        result += p;
    };
    (append(args), ...);
    return result;
}

#endif //FASTENGINE_ASSETS_H
