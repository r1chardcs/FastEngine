cmake_minimum_required(VERSION 4.3)
project(FastEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(FastEngine main.cpp
        Engine/App.cpp
        Engine/App.h
        Engine/Window.cpp
        Engine/Window.h
        Include/Platform.h
        Toolkit/Debug/Profiler.cpp
        Toolkit/Debug/Profiler.h
        Toolkit/Debug/Logger.cpp
        Toolkit/Debug/Logger.h
        Toolkit/Debug/Test.cpp
        Toolkit/Debug/Test.h
        Toolkit/IO/IO.cpp
        Toolkit/IO/IO.h
        Engine/RenderSystem.cpp
        Engine/RenderSystem.h
        Engine/UIContext.cpp
        Engine/UIContext.h
        Toolkit/Color.cpp
        Toolkit/Color.h
        Toolkit/Vector.h
        Engine/BufferBuilder.cpp
        Engine/BufferBuilder.h
        Engine/Render2D.cpp
        Engine/Render2D.h
        Toolkit/Assets/Assets.cpp
        Toolkit/Assets/Assets.h
        Engine/Object.cpp
        Engine/Object.h
        Engine/Component.cpp
        Engine/Component.h
        Engine/components/Transform.cpp
        Engine/components/Transform.h
        Toolkit/Quat.h
        Engine/objects/Rect.cpp
        Engine/objects/Rect.h
        Engine/components/Sprite.cpp
        Engine/components/Sprite.h
)

target_link_libraries(FastEngine PUBLIC gdi32 opengl32 dbghelp ${CMAKE_SOURCE_DIR}/Libraries/libglfw3.lib)
include_directories(Include)
