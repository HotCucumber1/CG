file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)


add_executable(
        Arcanoid3D
        ../../Common/StbImpl.cpp
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        Model/Ball.cpp
        Model/Brick.cpp
        Model/Paddle.cpp
        Model/Level.cpp
        Model/GameModel.cpp
        View/GameView.cpp
        main.cpp)

target_include_directories(Arcanoid3D PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(Arcanoid3D PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(Arcanoid3D PRIVATE ${GLEW_LIBRARIES})
