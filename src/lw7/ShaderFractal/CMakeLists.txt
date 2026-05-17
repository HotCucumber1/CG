file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)


add_executable(
        ShaderFractal
        ../../Common/StbImpl.cpp
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        SquareMesh.cpp
        Window.cpp
        main.cpp)

target_include_directories(ShaderFractal PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(ShaderFractal PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(ShaderFractal PRIVATE ${GLEW_LIBRARIES})
