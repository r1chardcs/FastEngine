cmake_minimum_required(VERSION 3.10)

project(FastBuild)

add_executable(FastBuild
        main.cpp
        EngineInf.h
        CompilerInf.h
        SysBuild.h
        SysBuild.cpp
        BuildTarget.cpp
        BuildTarget.h
        lang/FastBuildLang.cpp
        lang/FastBuildLang.h
        lang/DefFunc.h
        lang/RuntimeFunc.h
        funcs/Print.h
        funcs/Build.h
        State.h
        funcs/CXX.h
        funcs/Target.h
)

set_target_properties(FastBuild PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin/"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin/"
)

target_link_libraries(FastBuild PUBLIC
        Toolkit
)
target_link_options(FastBuild PRIVATE -Wl,--pdb=)
target_include_directories(FastBuild PUBLIC
        ../Include
)

add_subdirectory(tests/1)