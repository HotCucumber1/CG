cmake_minimum_required(VERSION 3.29)
project(CG)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glfw3 REQUIRED)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)


add_subdirectory(src/lw3/Example)
add_subdirectory(src/lw3/Asteroids)
