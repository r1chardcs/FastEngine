cmake_minimum_required(VERSION 3.10)
project(Toolkit)

add_library(Toolkit STATIC
        Anim/AnimationSystem.cpp
        Random.cpp
        Input.cpp
        Debug/AllocatorProfiler.cpp
        Debug/LagProfiler.cpp
        data/LocalStorage.cpp
        Anim/Easy.cpp
        Box2D.cpp
        Debug/SelfCrashDumper.cpp
        Assets/Assets.cpp
        Color.cpp
        IO/IO.cpp
        Debug/Test.cpp
        Debug/Logger.cpp
        Debug/Profiler.cpp
        TUI/TuiApplication.cpp
        TUI/TuiApplication.h
        TUI/ConsoleWriter.cpp
        TUI/ConsoleWriter.h
)

target_link_libraries(Toolkit PUBLIC dbghelp)
target_include_directories(Toolkit PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(Toolkit PUBLIC ../Include)


set_target_properties(Toolkit PROPERTIES
        PREFIX "lib"
        OUTPUT_NAME "Toolkit"

        ARCHIVE_OUTPUT_DIRECTORY
        "${CMAKE_SOURCE_DIR}/bin/core/libs"

        ARCHIVE_OUTPUT_DIRECTORY_DEBUG
        "${CMAKE_SOURCE_DIR}/bin/core/libs"

        ARCHIVE_OUTPUT_DIRECTORY_RELEASE
        "${CMAKE_SOURCE_DIR}/bin/core/libs"

        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO
        "${CMAKE_SOURCE_DIR}/bin/core/libs"

        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL
        "${CMAKE_SOURCE_DIR}/bin/core/libs"
)

target_include_directories(Toolkit
        PUBLIC
        "${CMAKE_SOURCE_DIR}/Toolkit"
        "${CMAKE_SOURCE_DIR}/Include"
        "${CMAKE_SOURCE_DIR}"
)