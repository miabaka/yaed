cmake_minimum_required(VERSION 3.20)

project(yaed)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory(third_party/fmt)
add_subdirectory(third_party/gl3w)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glm)
add_subdirectory(third_party/imgui)
add_subdirectory(third_party/json)
add_subdirectory(third_party/stb)

add_subdirectory(cuteshell)

add_subdirectory(editor)