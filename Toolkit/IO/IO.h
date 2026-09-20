//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_IO_H
#define FASTENGINE_IO_H
#include <Platform.h>

using MessageBoxFlags_t = std::uint32_t;

enum class MessageBoxStyle : MessageBoxFlags_t
{
    Ok                  = 0x00000000L,
    OkCancel            = 0x00000001L,
    AbortRetryIgnore    = 0x00000002L,
    YesNoCancel         = 0x00000003L,
    YesNo               = 0x00000004L,
    RetryCancel         = 0x00000005L,
    CancelTryContinue   = 0x00000006L,
    Help                = 0x00004000L,

    IconError           = 0x00000010L,
    IconQuestion        = 0x00000020L,
    IconWarning         = 0x00000030L,
    IconInformation     = 0x00000040L,

    IconHand            = 0x00000010L,
    IconStop            = 0x00000010L,
    IconExclamation     = 0x00000030L,
    IconAsterisk        = 0x00000040L,

    DefButton1          = 0x00000000L,
    DefButton2          = 0x00000100L,
    DefButton3          = 0x00000200L,
    DefButton4          = 0x00000300L,

    ApplModal           = 0x00000000L,
    SystemModal         = 0x00001000L,
    TaskModal           = 0x00002000L,

    DefaultDesktopOnly  = 0x00020000L,
    Right               = 0x00080000L,
    RtlReading          = 0x00100000L,
    SetForeground       = 0x00010000L,
    Topmost             = 0x00040000L,
    ServiceNotification = 0x00200000L,
    NoFocus             = 0x00008000L,
};

constexpr MessageBoxStyle operator|(MessageBoxStyle a, MessageBoxStyle b) noexcept
{
    return static_cast<MessageBoxStyle>(
        static_cast<MessageBoxFlags_t>(a) | static_cast<MessageBoxFlags_t>(b));
}

constexpr MessageBoxStyle operator&(MessageBoxStyle a, MessageBoxStyle b) noexcept
{
    return static_cast<MessageBoxStyle>(
        static_cast<MessageBoxFlags_t>(a) & static_cast<MessageBoxFlags_t>(b));
}

constexpr MessageBoxStyle operator^(MessageBoxStyle a, MessageBoxStyle b) noexcept
{
    return static_cast<MessageBoxStyle>(
        static_cast<MessageBoxFlags_t>(a) ^ static_cast<MessageBoxFlags_t>(b));
}

constexpr MessageBoxStyle operator~(MessageBoxStyle a) noexcept
{
    return static_cast<MessageBoxStyle>(~static_cast<MessageBoxFlags_t>(a));
}

inline MessageBoxStyle& operator|=(MessageBoxStyle& a, MessageBoxStyle b) noexcept
{
    a = a | b;
    return a;
}

inline MessageBoxStyle& operator&=(MessageBoxStyle& a, MessageBoxStyle b) noexcept
{
    a = a & b;
    return a;
}

constexpr MessageBoxFlags_t ToFlags(MessageBoxStyle style) noexcept
{
    return static_cast<MessageBoxFlags_t>(style);
}

namespace IO {
    STRING InputString(const STRING & = "");
    CHAR InputChar(const STRING & = "");
    void MsgBox(const STRING& title, const STRING& text, MessageBoxFlags_t style);

    namespace File {
        Err<STRING> ReadFile(const STRING& path);
        Err<VECTOR<BYTE>> ReadFileBytes(const STRING& path);

        Err<BOOL> ExistFile(const STRING& path);
        Err<BOOL> ExistDirectory(const STRING& path);

        Err<NOT> WriteFile(const STRING& path, const STRING& content);
        Err<NOT> CreateDirectory(const STRING& path);
    }


};


#endif //FASTENGINE_IO_H
