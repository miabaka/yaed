project(cutegl-example)

add_executable(cutegl-example
        src/main.cpp)

target_link_libraries(cutegl-example PRIVATE cutegl)
