//
// Created by dlllibstdntc on 17.09.2026.
//

#include "CrashDumper.h"

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

namespace {
    FUNC<VOID(CrashContext)> crash_dumper_callback;
    MUTEX symbol_mutex;

    STRING DescribeException(DWORD code) {
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

    VECTOR<StackFrame> CaptureStackTrace(PCONTEXT context) {
        VECTOR<StackFrame> frames;

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

            StackFrame frame;
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
}

LONG WINAPI UnhandledExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo) {
    CrashContext ctx;
    ctx.exception_address = ExceptionInfo->ExceptionRecord->ExceptionAddress;
    ctx.exception_description = DescribeException(ExceptionInfo->ExceptionRecord->ExceptionCode);
    ctx.stack_trace = CaptureStackTrace(ExceptionInfo->ContextRecord);

    if (crash_dumper_callback) {
        crash_dumper_callback(ctx);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

void CrashDumper::SetCallback(const FUNC<VOID(CrashContext)> &callback) {
    crash_dumper_callback = callback;
}

void CrashDumper::AttachHandler() {
    AddVectoredExceptionHandler(1, UnhandledExceptionHandler);
}