project(stb)

add_library(stb_image STATIC src/image.c)

target_include_directories(stb_image PUBLIC include)