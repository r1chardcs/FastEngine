cmake_minimum_required(VERSION 3.10)

project(TheNightKnight)

add_executable(TheNightKnight App.cpp
        TheNightKnight.cpp
        TheNightKnight.h
        scenes/MainScene.cpp
        scenes/MainScene.h
        scripts/entity/LivingEntity.cpp
        scripts/entity/LivingEntity.h
        scripts/entity/Player.cpp
        scripts/entity/Player.h
        scripts/FPSOverlay.cpp
        scripts/FPSOverlay.h
        scripts/block/AbstractBlock.cpp
        scripts/block/AbstractBlock.h
        scripts/block/ColorBlock.cpp
        scripts/block/ColorBlock.h
        scripts/map/MapGenerator.cpp
        scripts/map/MapGenerator.h
        scripts/entity/enemy/Denuvo.cpp
        scripts/entity/enemy/Denuvo.h
        scripts/entity/EnemyEntity.cpp
        scripts/entity/EnemyEntity.h
        scripts/entity/ai/EnemyState.cpp
        scripts/entity/ai/EnemyState.h
        scripts/entity/ai/MeleeAIStates.cpp
        scripts/entity/ai/MeleeAIStates.h
        scenes/DieScene.cpp
        scenes/DieScene.h
        scenes/LoadScene.cpp
        scenes/LoadScene.h
        scripts/weapon/Weapon.cpp
        scripts/weapon/Weapon.h
        scripts/weapon/sword/OxidativeSword.cpp
        scripts/weapon/sword/OxidativeSword.h)

target_link_libraries(TheNightKnight PUBLIC FastEngine)

add_custom_command(TARGET TheNightKnight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:FastEngine>"
        "$<TARGET_FILE_DIR:TheNightKnight>"
)


target_include_directories(TheNightKnight PUBLIC ${CMAKE_SOURCE_DIR})
target_link_options(TheNightKnight PRIVATE -Wl,--pdb=)