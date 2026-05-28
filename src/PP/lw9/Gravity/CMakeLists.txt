file(
        COPY data
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)

add_executable(Gravity
        ../../../Common/StbImpl.cpp
        ../../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../../Common/BaseWindow/BaseWindow.cpp
        ParticleSystem.cpp
        Camera.cpp
        Window.cpp
        main.cpp)

target_include_directories(Gravity PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(Gravity PRIVATE OpenCL::OpenCL)
target_link_libraries(Gravity PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(Gravity PRIVATE ${GLEW_LIBRARIES})
