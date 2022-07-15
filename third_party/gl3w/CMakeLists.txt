project(gl3w)

add_library(gl3w STATIC src/gl3w.c)

target_include_directories(gl3w PUBLIC include)

target_link_libraries(gl3w PRIVATE ${CMAKE_DL_LIBS})