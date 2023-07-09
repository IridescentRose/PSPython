cmake_minimum_required(VERSION 3.15)

project(PSPython)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${SOURCES})

if(PSP)
    target_link_libraries(${PROJECT_NAME} stdc++ pspdebug pspdisplay pspge pocketpy)

    create_pbp_file(
            TARGET ${PROJECT_NAME}
            ICON_PATH NULL
            BACKGROUND_PATH NULL
            PREVIEW_PATH NULL
            TITLE "Python Test"
            BUILD_PRX
    )
endif()