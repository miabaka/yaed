cmake_minimum_required(VERSION 3.20)

project(yaed)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(gl3w)
add_subdirectory(imgui)
add_subdirectory(editor)