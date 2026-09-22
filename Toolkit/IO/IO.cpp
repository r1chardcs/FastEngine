//
// Created by dlllibstdntc on 12.09.2026.
//

#include "IO.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ostream>
#include <windows.h>

STRING IO::InputString(const STRING &msg) {
    if (!msg.empty())
        std::cout << msg;

    STRING input;
    std::getline(std::cin, input);
    return input;
}

CHAR IO::InputChar(const STRING &msg) {
    if (!msg.empty())
        std::cout << msg;

    char c = 0;
    std::cin.get(c);
    return c;
}

#undef MessageBox
void IO::MsgBox(const STRING &title, const STRING &text, MessageBoxFlags_t style) {
    ::MessageBoxA(
        nullptr,
        text.c_str(),
        title.c_str(),
        static_cast<UINT>(style)
    );
}

Err<STRING> IO::File::ReadFile(const STRING &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        return {.res = {}, .err = "failed to open file for reading" };

    std::ostringstream ss;
    ss << file.rdbuf();
    if (file.bad())
        return {.res = {}, .err = "failed to read file" };

    return { .res = ss.str(), .err = nullptr };
}

Err<VECTOR<BYTE>> IO::File::ReadFileBytes(const STRING &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
        return { .res = {}, .err = "failed to open file for reading" };

    const std::streamsize size = file.tellg();
    if (size < 0)
        return { .res = {}, .err = "failed to determine file size" };

    file.seekg(0, std::ios::beg);

    VECTOR<BYTE> buffer(static_cast<size_t>(size));
    if (size > 0 && !file.read(reinterpret_cast<char*>(buffer.data()), size))
        return { .res = {}, .err = "failed to read file" };

    return { .res = std::move(buffer), .err = nullptr };
}

Err<BOOL> IO::File::ExistFile(const STRING &path) {
    try {
        if (!std::filesystem::exists(path))
            return { .res = false, .err = nullptr };
        if (!std::filesystem::is_regular_file(path))
            return { .res = false, .err = nullptr };
        return { .res = true, .err = nullptr };
    } catch (const std::filesystem::filesystem_error& e) {
        return { .res = false, .err = e.what() };
    }
}

Err<BOOL> IO::File::ExistDirectory(const STRING &path) {
    try {
        if (!std::filesystem::exists(path))
            return { .res = false, .err = nullptr };
        if (!std::filesystem::is_directory(path))
            return { .res = false, .err = nullptr };
        return { .res = true, .err = nullptr };
    } catch (const std::filesystem::filesystem_error& e) {
        return { .res = false, .err = e.what() };
    }
}

Err<NOT> IO::File::WriteFile(const STRING &path, const STRING &content) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return {.res = NOT{}, .err = "Failed to open file for writing"};
    }

    file.write(content.data(), static_cast<std::streamsize>(content.size()));
    if (!file.good()) {
        return {.res = NOT{}, .err = "Failed to write file contents"};
    }

    file.close();
    if (file.fail()) {
        return {.res = NOT{}, .err = "Failed to flush/close file"};
    }

    return {.res = NOT{}, .err = nullptr};
}

#undef CreateDirectory
#undef CopyFile
#undef CopyDir

Err<NOT> IO::File::CreateDirectory(const STRING &path) {
    try {
        std::filesystem::create_directories(path);
        return {.res = {}, .err = nullptr };
    } catch (const std::filesystem::filesystem_error& e) {
        return {.res = {}, .err = e.what() };
    }
}

Err<NOT> IO::File::CopyFile(const STRING &from, const STRING &to) {
    try {
        if (const std::filesystem::path toPath(to); toPath.has_parent_path()) {
            std::error_code ec;
            std::filesystem::create_directories(toPath.parent_path(), ec);
        }

        std::filesystem::copy_file(
            from,
            to,
            std::filesystem::copy_options::overwrite_existing
        );
        return {.res = {}, .err = nullptr};
    } catch (const std::filesystem::filesystem_error& e) {
        static thread_local STRING lastError;
        lastError = e.what();
        return {.res = {}, .err = lastError.c_str()};
    }
}

Err<NOT> IO::File::CopyDir(const STRING &from, const STRING &to) {
    try {
        std::filesystem::create_directories(to);
        std::filesystem::copy(
            from,
            to,
            std::filesystem::copy_options::recursive |
            std::filesystem::copy_options::overwrite_existing
        );
        return {.res = {}, .err = nullptr};
    } catch (const std::filesystem::filesystem_error& e) {
        return {.res = {}, .err = e.what()};
    }
}