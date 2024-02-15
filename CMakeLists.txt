cmake_minimum_required(VERSION 3.20)

project(yaed)

add_subdirectory(third_party/fmt)
add_subdirectory(third_party/gl3w)
add_subdirectory(third_party/glm)
add_subdirectory(third_party/imgui)
add_subdirectory(third_party/json)
add_subdirectory(third_party/SDL)
add_subdirectory(third_party/stb)

add_subdirectory(cuteshell)

add_subdirectory(editor)