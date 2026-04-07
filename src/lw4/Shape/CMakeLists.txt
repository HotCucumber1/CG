add_executable(Shape
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        Shape.cpp
        main.cpp)

target_link_libraries(Shape PRIVATE OpenGL::GL glfw OpenGL::GLU)
