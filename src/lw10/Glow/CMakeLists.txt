file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)

add_executable(
        Glow
        ../../Common/StbImpl.cpp
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        App.cpp
        main.cpp
)

target_include_directories(Glow PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(Glow PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(Glow PRIVATE ${GLEW_LIBRARIES})
