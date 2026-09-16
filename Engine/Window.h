//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_WINDOW_H
#define FASTENGINE_WINDOW_H

#include "../Include/Platform.h"

class Window final {
    HANDLE handle = nullptr;
    CALLBACK<VOID(VIEW_PTR<Window>, INT, INT)> resize_callback = nullptr;
    STRING title;
    INT width, height;
    void SetWindowSize(INT _width, INT _height);
public:
    Window(MOVE_PLEASE STRING title, INT width, INT height);

    void MakeContext();
    BOOL IsRun() const;
    void SwapBuffer() const;

    void SetResizeCallback(const CALLBACK<VOID(VIEW_PTR<Window>, INT, INT)> &callback);

    HANDLE GetHandle() const;
    ~Window();

    DOUBLE GetTime();
    INT GetWidth() const;
    INT GetHeight() const;
};


#endif //FASTENGINE_WINDOW_H
