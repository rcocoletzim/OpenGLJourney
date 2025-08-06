cmake_minimum_required(VERSION 4.0)

find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)

add_executable(HelloTriangle main.cpp)

target_link_libraries(HelloTriangle PRIVATE glfw GLEW::glew_s)