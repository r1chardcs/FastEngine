cmake_minimum_required(VERSION 3.10)

project(TheNightKnight)

add_executable(TheNightKnight App.cpp
        TheNightKnight.cpp
        TheNightKnight.h)

target_link_libraries(TheNightKnight PUBLIC FastEngine)

add_custom_command(TARGET TheNightKnight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:FastEngine>"
        "$<TARGET_FILE_DIR:TheNightKnight>"
)