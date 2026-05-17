file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)


add_executable(
        ShaderTransform
        ../../Common/StbImpl.cpp
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        Grid.cpp
        Window.cpp
        main.cpp)

target_include_directories(ShaderTransform PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(ShaderTransform PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(ShaderTransform PRIVATE ${GLEW_LIBRARIES})
