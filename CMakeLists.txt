cmake_minimum_required(VERSION 3.15)
project(Ballistics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.2.0
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)
FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
    FetchContent_Populate(stb)
endif()

set(SOURCES
    src/main.cpp
    src/simulation/Projectile.cpp
    src/simulation/Environment.cpp
    src/simulation/Simulator.cpp
    src/renderer/Renderer.cpp
    src/input/InputHandler.cpp
)

add_executable(ballistics ${SOURCES})

target_include_directories(ballistics PRIVATE src ${stb_SOURCE_DIR})

target_link_libraries(ballistics PRIVATE SDL3::SDL3)
