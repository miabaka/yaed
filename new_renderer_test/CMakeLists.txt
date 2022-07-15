project(new_renderer_test)

add_executable(new_renderer_test src/main.cpp)

target_link_libraries(new_renderer_test glfw gl3w glm stb_image)