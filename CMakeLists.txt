cmake_minimum_required(VERSION 4.3)
project(FastEngine)

set(CMAKE_CXX_STANDARD 20)

add_library(FastEngine SHARED main.cpp
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
        Engine/GameObject.cpp
        Engine/GameObject.h
        Engine/Component.cpp
        Engine/Component.h
        Engine/components/Transform.cpp
        Engine/components/Transform.h
        Toolkit/Quat.h
        Engine/objects/Rect.cpp
        Engine/objects/Rect.h
        Engine/components/Sprite.cpp
        Engine/components/Sprite.h
        Toolkit/Debug/SelfCrashDumper.cpp
        Toolkit/Debug/SelfCrashDumper.h
        Engine/components/HitboxBox2D.cpp
        Engine/components/HitboxBox2D.h
        Toolkit/Box2D.cpp
        Toolkit/Box2D.h
        Engine/UILayer.cpp
        Engine/UILayer.h
        Engine/Scene.cpp
        Engine/Scene.h
        Toolkit/Anim/Easy.cpp
        Toolkit/Anim/Easy.h
        Toolkit/Anim/Lerp.h
        "Toolkit/Anim/ITween.h"
        Toolkit/Anim/Tween.h
        Toolkit/Anim/AnimationSystem.h
        Toolkit/Anim/AnimationSystem.cpp
        Toolkit/Anim/Animation.h
        Toolkit/data/LocalStorage.cpp
        Toolkit/data/LocalStorage.h
        Toolkit/data/ICipher.h
        Toolkit/data/NoCipher.h
        Toolkit/data/XorCipher.h
        Toolkit/data/Rc4Cipher.h
        "Toolkit/data/StorageSerializer.h"
        Toolkit/Debug/LagProfiler.cpp
        Toolkit/Debug/LagProfiler.h
        Toolkit/Debug/MemoryProfiler.cpp
        Toolkit/Debug/MemoryProfiler.h
)

target_link_libraries(FastEngine PUBLIC gdi32 opengl32 dbghelp ${CMAKE_SOURCE_DIR}/Libraries/libglfw3.lib)
target_include_directories(FastEngine PUBLIC Include)
target_include_directories(FastEngine PUBLIC ${CMAKE_SOURCE_DIR})

add_subdirectory(TheNightKnight)