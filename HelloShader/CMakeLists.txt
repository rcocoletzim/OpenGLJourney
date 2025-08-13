cmake_minimum_required(VERSION 4.0)

find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)

add_executable(HelloShader main.cpp ShaderProgram.cpp)

target_link_libraries(HelloShader PRIVATE glfw GLEW::glew_s)