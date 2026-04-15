cmake_minimum_required(VERSION 3.29)
project(CG)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)
FetchContent_MakeAvailable(stb)
include_directories(${stb_SOURCE_DIR})

add_subdirectory(src/Example)
add_subdirectory(src/lw3/Asteroids)

add_subdirectory(src/lw4/Shape)
add_subdirectory(src/lw4/Shape3)

add_subdirectory(src/lw5/Arcanoid3D)
