//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_WINDOW_H
#define FASTENGINE_WINDOW_H

#include "../Include/Platform.h"
#include "Toolkit/Vector.h"

class Window final {
    HANDLE handle = nullptr;
    FUNC<VOID(VIEW_PTR<Window>, INT, INT)> resize_callback = nullptr;
    STRING title;
    INT width, height;
    void SetWindowSize(INT _width, INT _height);
public:
    Window(MOVE_PLEASE STRING title, INT width, INT height);

    INT GetKey(INT key) const;
    INT GetMouseKey(INT key) const;
    toolkit::Vec2f GetMousePosition() const;

    void MakeContext();
    BOOL IsRun() const;
    void SwapBuffer() const;

    void SetResizeCallback(const FUNC<VOID(VIEW_PTR<Window>, INT, INT)> &callback);

    HANDLE GetHandle() const;
    ~Window();

    static DOUBLE GetTime();
    INT GetWidth() const;
    INT GetHeight() const;
};


#endif //FASTENGINE_WINDOW_H
