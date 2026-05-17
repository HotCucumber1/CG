file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)


add_executable(
        ShaderFlag
        ../../Common/StbImpl.cpp
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        SquareMesh.cpp
        Window.cpp
        main.cpp)

target_include_directories(ShaderFlag PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(ShaderFlag PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(ShaderFlag PRIVATE ${GLEW_LIBRARIES})
