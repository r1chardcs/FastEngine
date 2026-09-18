//
// Created by dlllibstdntc on 12.09.2026.
//

#include "Window.h"
#include <glfw3.h>

#include "../Toolkit/Debug/Logger.h"
#include "Toolkit/Debug/Test.h"

void Window::SetWindowSize(const INT _width, const INT _height) {
    this->width = _width;
    this->height = _height;

    if (resize_callback) resize_callback(this, width, height);
}

Window::Window(MOVE_PLEASE STRING title, const INT width, const INT height) : title(
                                                                      MOVE(title)), width(width), height(height) {
    TEST(!title.empty() && "Invalid Title")
    glfwInit();
    handle = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);
    if (!handle) {
        LOGERR.Output("Error create window '%s' (%d:%d)", this->title.c_str(), width, height);
    }
}

INT Window::GetKey(const INT key) const {
    TEST(!handle)
    return handle == nullptr? -1 : (glfwGetKey(static_cast<GLFWwindow *>(handle), key) != GLFW_RELEASE);
}

INT Window::GetMouseKey(const INT key) const {
    TEST(!handle);
    return glfwGetMouseButton(static_cast<GLFWwindow *>(handle), key) != GLFW_RELEASE;
}

Vec2f Window::GetMousePosition() const {
    TEST(!handle);
    double x, y;
    glfwGetCursorPos(static_cast<GLFWwindow *>(handle), &x, &y);
    return { static_cast<float>(x), static_cast<float>(y) };
}

void Window::MakeContext()  {
    TEST(!handle);
    glfwMakeContextCurrent(static_cast<GLFWwindow *>(handle));
    glfwSetWindowUserPointer(static_cast<GLFWwindow *>(handle), this);
    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(handle), [](auto handle, auto width, auto height) {
        if (auto self = static_cast<Window *>(glfwGetWindowUserPointer(handle))) {
            self->SetWindowSize(width, height);
        }
    });
    SetWindowSize(width, height); /*first call OnResize*/
}

BOOL Window::IsRun() const {
    TEST(!handle);
    return !glfwWindowShouldClose(
        static_cast<GLFWwindow *>(handle));
}

void Window::SwapBuffer() const {
    TEST(!handle);
    glfwSwapBuffers(static_cast<GLFWwindow *>(handle));
    glfwPollEvents();
}

void Window::SetResizeCallback(const CALLBACK<void(VIEW_PTR<Window>, INT, INT)> &callback) {
    this->resize_callback = callback;
}

HANDLE Window::GetHandle() const {
    return handle;
}

Window::~Window() {
    if (handle) {
        glfwDestroyWindow(static_cast<GLFWwindow *>(handle));
    }
    glfwTerminate();
}

DOUBLE Window::GetTime() {
    return glfwGetTime();
}

INT Window::GetWidth() const {
    return width;
}

INT Window::GetHeight() const {
    return height;
}
