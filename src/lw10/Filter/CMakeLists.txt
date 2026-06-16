file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)

add_executable(Filter
        ../../Common/StbImpl.cpp
        ../../Common/GLFWInitializer/GLFWInitializer.cpp
        ../../Common/BaseWindow/BaseWindow.cpp
        EdgeDetectionWindow.cpp
        main.cpp)

target_include_directories(Filter PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(Filter PRIVATE OpenGL::GL glfw OpenGL::GLU)
target_link_libraries(Filter PRIVATE ${GLEW_LIBRARIES})
