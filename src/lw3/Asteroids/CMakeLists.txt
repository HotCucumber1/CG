file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data/
)

add_executable(Asteroids
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        ../../Common/Audio/Audio.cpp
        Entity/Bullet.cpp
        Entity/Asteroid.cpp
        AsteroidsView.cpp
        AsteroidsGame.cpp
        main.cpp)

target_link_libraries(Asteroids PRIVATE OpenGL::GL glfw OpenAL::OpenAL)
