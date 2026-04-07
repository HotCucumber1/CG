add_executable(ExampleGL
        ../Common/GLFWInitializer/GLFWInitializer.cpp
        ../Common/BaseWindow/BaseWindow.cpp
        main.cpp)

target_link_libraries(ExampleGL PRIVATE OpenGL::GL glfw)
