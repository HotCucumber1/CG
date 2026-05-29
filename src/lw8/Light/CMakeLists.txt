file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)


add_executable(
        Light
        ../../Common/StbImpl.cpp
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        RayTracer.cpp
        Utils/Torus.cpp
        Window.cpp
        main.cpp
)

target_include_directories(Light PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(Light PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(Light PRIVATE ${GLEW_LIBRARIES})
