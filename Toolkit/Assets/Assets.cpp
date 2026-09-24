//
// Created by dlllibstdntc on 16.09.2026.
//

#include "Assets.h"

#include <filesystem>

#include "../IO/IO.h"

/*
 *  todo: полностью переписать Assets, перевести их в нормльный и легко использованный апи:
 *  todo: 1. на данный момент он не нужен, 2. он нигде не используется.
 */
VECTOR<BYTE> AssetFile::GetBytes() {
    DEPRECTED_API
    // return IO::File::ReadFileBytes(path).res;
}

STRING AssetFile::GetString() const {
    DEPRECTED_API
    // return IO::File::ReadFile(path).res;
}

Assets::Assets(STRING dir) : dir(MOVE(dir)) {
}

Err<AssetFile> Assets::GetFile(const STRING &path) {
    DEPRECTED_API
    /*
    STRING fullPath = CombinePath(dir, path);

    auto bytes = IO::File::ReadFileBytes(fullPath);
    if (bytes.err != nullptr)
        return { .res = {}, .err = bytes.err };

    AssetFile file;
    file.path = fullPath;
    file.name = std::filesystem::path(fullPath).stem().string();
    file.extension = std::filesystem::path(fullPath).extension().string();

    return { .res = std::move(file), .err = nullptr };
    */
}