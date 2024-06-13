project(cuteshell_sdl2)

add_library(cuteshell_sdl2 STATIC
        include/cute/shell/sdl2.hpp
        src/sdl2.cpp)

target_include_directories(cuteshell_sdl2 PUBLIC include)

target_link_libraries(cuteshell_sdl2
        PUBLIC
        cuteshell
        SDL2-static)