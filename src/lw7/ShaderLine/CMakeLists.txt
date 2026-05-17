file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)


add_executable(
        ShaderLine
        ../../Common/StbImpl.cpp
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        LineMash.cpp
        Window.cpp
        main.cpp)

target_include_directories(ShaderLine PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(ShaderLine PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(ShaderLine PRIVATE ${GLEW_LIBRARIES})
