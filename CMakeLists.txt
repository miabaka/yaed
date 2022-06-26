cmake_minimum_required(VERSION 3.20)

project(yaed)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory(fmt)
add_subdirectory(json)
add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(gl3w)
add_subdirectory(imgui)
add_subdirectory(glfw)

add_subdirectory(cuteshell)
add_subdirectory(cutegl)
add_subdirectory(cutegl-example)
add_subdirectory(editor)