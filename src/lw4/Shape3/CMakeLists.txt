add_executable(Shape3
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        MobiusStrip.cpp
        Shape3.cpp
        main.cpp)

target_link_libraries(Shape3 PRIVATE OpenGL::GL glfw OpenGL::GLU)
