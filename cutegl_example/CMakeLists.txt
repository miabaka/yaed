project(cutegl_example)

add_executable(cutegl_example
        src/main.cpp)

target_link_libraries(cutegl_example PRIVATE cutegl)
