//
// Created by dlllibstdntc on 17.09.2026.
//

#include "SelfCrashDumper.h"

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

static FUNC<VOID(toolkit::CrashContext)> crash_dumper_callback;
static MUTEX symbol_mutex;

STRING toolkit::no_new::DescribeException(DWORD code) {
    switch (code) {
        case EXCEPTION_ACCESS_VIOLATION: return "Access violation";
        case EXCEPTION_STACK_OVERFLOW: return "Stack overflow";
        case EXCEPTION_ILLEGAL_INSTRUCTION: return "Illegal instruction";
        case EXCEPTION_INT_DIVIDE_BY_ZERO: return "Integer divide by zero";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "Float divide by zero";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "Array bounds exceeded";
        case EXCEPTION_PRIV_INSTRUCTION: return "Privileged instruction";
        case EXCEPTION_IN_PAGE_ERROR: return "In-page I/O error";
        default: {
            char buf[32];
            snprintf(buf, sizeof(buf), "0x%08lX", code);
            return STRING("Unknown exception ") + buf;
        }
    }
}


VECTOR<toolkit::StackFrame> CaptureStackTrace(PCONTEXT context) {
    VECTOR<toolkit::StackFrame> frames;

    const HANDLE process = GetCurrentProcess();
    const HANDLE thread = GetCurrentThread();

    MUTEX_LOCK lock(symbol_mutex);

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
    if (!SymInitialize(process, nullptr, TRUE)) {
        return frames;
    }

    STACKFRAME64 stackFrame = {};
    DWORD machineType;

#if defined(_M_X64)
    machineType = IMAGE_FILE_MACHINE_AMD64;
    stackFrame.AddrPC.Offset = context->Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#elif defined(_M_IX86)
    machineType = IMAGE_FILE_MACHINE_I386;
    stackFrame.AddrPC.Offset = context->Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#else
#error "Unsupported architecture"
#endif

    constexpr INT MAX_FRAMES = 64;

    for (INT i = 0; i < MAX_FRAMES; ++i) {
        if (!StackWalk64(machineType, process, thread, &stackFrame, context,
                          nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) {
            break;
        }

        if (stackFrame.AddrPC.Offset == 0) {
            break;
        }

        toolkit::StackFrame frame;
        frame.address = reinterpret_cast<POINTER>(stackFrame.AddrPC.Offset);

        alignas(SYMBOL_INFO) BYTE symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(CHAR)];
        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64 displacement = 0;
        if (SymFromAddr(process, stackFrame.AddrPC.Offset, &displacement, symbol)) {
            frame.function_name = symbol->Name;
        } else {
            frame.function_name = "<unknown function>";
        }

        IMAGEHLP_LINE64 line = {};
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        DWORD lineDisplacement = 0;
        if (SymGetLineFromAddr64(process, stackFrame.AddrPC.Offset, &lineDisplacement, &line)) {
            frame.file_name = line.FileName;
            frame.line_number = line.LineNumber;
        }

        IMAGEHLP_MODULE64 moduleInfo = {};
        moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
        if (SymGetModuleInfo64(process, stackFrame.AddrPC.Offset, &moduleInfo)) {
            frame.module_name = moduleInfo.ModuleName;
        }

        frames.push_back(MOVE(frame));
    }

    SymCleanup(process);
    return frames;
}

toolkit::no_new::StackTrace toolkit::no_new::CaptureStackTraceCurrent() {
    StackTrace result;

    CONTEXT context = {};
    RtlCaptureContext(&context);

    const HANDLE process = GetCurrentProcess();
    const HANDLE thread = GetCurrentThread();

    MUTEX_LOCK lock(symbol_mutex);

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

    if (!SymInitialize(process, nullptr, TRUE)) {
        return result;
    }

    STACKFRAME64 stackFrame = {};
    DWORD machineType = 0;

#if defined(_M_X64)

    machineType = IMAGE_FILE_MACHINE_AMD64;

    stackFrame.AddrPC.Offset = context.Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;

    stackFrame.AddrFrame.Offset = context.Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;

    stackFrame.AddrStack.Offset = context.Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;

#elif defined(_M_IX86)

    machineType = IMAGE_FILE_MACHINE_I386;

    stackFrame.AddrPC.Offset = context.Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;

    stackFrame.AddrFrame.Offset = context.Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;

    stackFrame.AddrStack.Offset = context.Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;

#else

#error "Unsupported architecture"

#endif

    for (SIZE_T i = 0; i < StackTrace::MAX_FRAMES; ++i) {
        if (!StackWalk64(
            machineType,
            process,
            thread,
            &stackFrame,
            &context,
            nullptr,
            SymFunctionTableAccess64,
            SymGetModuleBase64,
            nullptr
        )) {
            break;
        }

        if (stackFrame.AddrPC.Offset == 0) {
            break;
        }

        StackFrame& frame = result.frames[result.count];

        frame.address =
            reinterpret_cast<POINTER>(stackFrame.AddrPC.Offset);

        // =========================================================
        // SYMBOL
        // =========================================================

        alignas(SYMBOL_INFO)
        BYTE symbolBuffer[
            sizeof(SYMBOL_INFO) + MAX_SYM_NAME
        ] = {};

        auto* symbol =
            reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);

        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64 displacement = 0;

        if (SymFromAddr(
            process,
            stackFrame.AddrPC.Offset,
            &displacement,
            symbol
        )) {
            const SIZE_T length =
                (symbol->NameLen < StackFrame::MAX_FUNCTION_NAME - 1)
                    ? symbol->NameLen
                    : StackFrame::MAX_FUNCTION_NAME - 1;

            memcpy(
                frame.function_name,
                symbol->Name,
                length
            );

            frame.function_name[length] = '\0';
        } else {
            const char* unknown = "<unknown function>";

            strcpy_s(
                frame.function_name,
                StackFrame::MAX_FUNCTION_NAME,
                unknown
            );
        }

        // =========================================================
        // SOURCE FILE / LINE
        // =========================================================

        IMAGEHLP_LINE64 line = {};
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        DWORD lineDisplacement = 0;

        if (SymGetLineFromAddr64(
            process,
            stackFrame.AddrPC.Offset,
            &lineDisplacement,
            &line
        )) {
            if (line.FileName) {
                strncpy_s(
                    frame.file_name,
                    StackFrame::MAX_FILE_NAME,
                    line.FileName,
                    _TRUNCATE
                );
            }

            frame.line_number = line.LineNumber;
        }
        IMAGEHLP_MODULE64 moduleInfo = {};
        moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

        if (SymGetModuleInfo64(
            process,
            stackFrame.AddrPC.Offset,
            &moduleInfo
        )) {
            strncpy_s(
                frame.module_name,
                StackFrame::MAX_MODULE_NAME,
                moduleInfo.ModuleName,
                _TRUNCATE
            );
        }

        ++result.count;
    }

    SymCleanup(process);

    return result;
}
LONG WINAPI UnhandledExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo) {
    toolkit::CrashContext ctx;
    ctx.exception_address = ExceptionInfo->ExceptionRecord->ExceptionAddress;
    ctx.exception_description = toolkit::no_new::DescribeException(ExceptionInfo->ExceptionRecord->ExceptionCode);
    ctx.stack_trace = CaptureStackTrace(ExceptionInfo->ContextRecord);

    if (crash_dumper_callback) {
        crash_dumper_callback(ctx);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

void toolkit::scd::SetCallback(const FUNC<VOID(CrashContext)> &callback) {
    crash_dumper_callback = callback;
}

void toolkit::scd::AttachHandler() {
    AddVectoredExceptionHandler(1, UnhandledExceptionHandler);
}