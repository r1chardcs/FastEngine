cmake_minimum_required(VERSION 4.3)

project(FastEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FASTENGINE_BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set(FASTENGINE_CORE_DIR "${FASTENGINE_BIN_DIR}/core")
set(FASTENGINE_INCLUDE_DIR "${FASTENGINE_CORE_DIR}/include")
set(FASTENGINE_LIB_DIR "${FASTENGINE_CORE_DIR}/libs")

file(MAKE_DIRECTORY
        "${FASTENGINE_BIN_DIR}"
        "${FASTENGINE_CORE_DIR}"
        "${FASTENGINE_INCLUDE_DIR}"
        "${FASTENGINE_LIB_DIR}"
)

add_library(FastEngine SHARED
        main.cpp

        Engine/App.cpp
        Engine/App.h

        Engine/Window.cpp
        Engine/Window.h

        Include/Platform.h

        Engine/RenderSystem.cpp
        Engine/RenderSystem.h

        Engine/UIContext.cpp
        Engine/UIContext.h

        Engine/BufferBuilder.cpp
        Engine/BufferBuilder.h

        Engine/Render2D.cpp
        Engine/Render2D.h

        Engine/GameObject.cpp
        Engine/GameObject.h

        Engine/Component.cpp
        Engine/Component.h

        Engine/components/Transform.cpp
        Engine/components/Transform.h

        Engine/objects/Rect.cpp
        Engine/objects/Rect.h

        Engine/components/Sprite.cpp
        Engine/components/Sprite.h

        Engine/components/HitboxBox2D.cpp
        Engine/components/HitboxBox2D.h

        Engine/UILayer.cpp
        Engine/UILayer.h

        Engine/Scene.cpp
        Engine/Scene.h
)

set_target_properties(FastEngine PROPERTIES
        PREFIX ""
        OUTPUT_NAME "FastEngine"

        RUNTIME_OUTPUT_DIRECTORY "${FASTENGINE_LIB_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${FASTENGINE_LIB_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${FASTENGINE_LIB_DIR}"
)

target_include_directories(FastEngine
        PUBLIC
        "${CMAKE_SOURCE_DIR}/Include"
        "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(FastEngine
        PUBLIC
        Toolkit
        gdi32
        opengl32
        dbghelp
        "${CMAKE_SOURCE_DIR}/Libraries/libglfw3.lib"
)

add_subdirectory(Toolkit)
add_subdirectory(FastBuild)


file(GLOB_RECURSE FASTENGINE_HEADERS
        CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/Engine/*.h"
)

file(GLOB_RECURSE TOOLKIT_HEADERS
        CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/Toolkit/*.h"
)

file(GLOB_RECURSE PLATFORM_HEADERS
        CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/Include/*.h"
)


add_custom_target(FastEngineHeaders ALL
        COMMAND ${CMAKE_COMMAND} -E rm -rf
        "${FASTENGINE_INCLUDE_DIR}"

        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${FASTENGINE_INCLUDE_DIR}/Engine"

        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${FASTENGINE_INCLUDE_DIR}/Toolkit"

        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${FASTENGINE_INCLUDE_DIR}/Include"

        COMMENT "Preparing FastEngine include directory..."
)


foreach(HEADER ${FASTENGINE_HEADERS})

    file(RELATIVE_PATH RELATIVE_HEADER
            "${CMAKE_SOURCE_DIR}/Engine"
            "${HEADER}"
    )

    get_filename_component(HEADER_DIRECTORY
            "${RELATIVE_HEADER}"
            DIRECTORY
    )

    if(HEADER_DIRECTORY)
        set(DESTINATION
                "${FASTENGINE_INCLUDE_DIR}/Engine/${HEADER_DIRECTORY}"
        )
    else()
        set(DESTINATION
                "${FASTENGINE_INCLUDE_DIR}/Engine"
        )
    endif()

    add_custom_command(
            TARGET FastEngineHeaders
            POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E make_directory
            "${DESTINATION}"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${HEADER}"
            "${DESTINATION}"
    )

endforeach()


foreach(HEADER ${TOOLKIT_HEADERS})

    file(RELATIVE_PATH RELATIVE_HEADER
            "${CMAKE_SOURCE_DIR}/Toolkit"
            "${HEADER}"
    )

    get_filename_component(HEADER_DIRECTORY
            "${RELATIVE_HEADER}"
            DIRECTORY
    )

    if(HEADER_DIRECTORY)
        set(DESTINATION
                "${FASTENGINE_INCLUDE_DIR}/Toolkit/${HEADER_DIRECTORY}"
        )
    else()
        set(DESTINATION
                "${FASTENGINE_INCLUDE_DIR}/Toolkit"
        )
    endif()

    add_custom_command(
            TARGET FastEngineHeaders
            POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E make_directory
            "${DESTINATION}"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${HEADER}"
            "${DESTINATION}"
    )

endforeach()


foreach(HEADER ${PLATFORM_HEADERS})

    file(RELATIVE_PATH RELATIVE_HEADER
            "${CMAKE_SOURCE_DIR}/Include"
            "${HEADER}"
    )

    get_filename_component(HEADER_DIRECTORY
            "${RELATIVE_HEADER}"
            DIRECTORY
    )

    if(HEADER_DIRECTORY)
        set(DESTINATION
                "${FASTENGINE_INCLUDE_DIR}/Include/${HEADER_DIRECTORY}"
        )
    else()
        set(DESTINATION
                "${FASTENGINE_INCLUDE_DIR}/Include"
        )
    endif()

    add_custom_command(
            TARGET FastEngineHeaders
            POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E make_directory
            "${DESTINATION}"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${HEADER}"
            "${DESTINATION}"
    )

endforeach()


add_dependencies(FastEngine FastEngineHeaders)