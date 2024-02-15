project(imgui)

add_library(imgui STATIC
        include/imgui/imconfig.h
        include/imgui/imgui.h
        include/imgui/imgui_impl_opengl3.h
        include/imgui/imgui_impl_sdl2.h
        include/imgui/imgui_stdlib.h
        include/imgui/imstb_textedit.h
        src/imgui.cpp
        src/imgui_demo.cpp
        src/imgui_draw.cpp
        src/imgui_impl_opengl3.cpp
        src/imgui_impl_sdl2.cpp
        src/imgui_stdlib.cpp
        src/imgui_tables.cpp
        src/imgui_widgets.cpp
        src/imstb_rectpack.h
        src/imstb_truetype.h)

target_include_directories(imgui PUBLIC include)

target_link_libraries(imgui
        PRIVATE
        gl3w
        PUBLIC
        glm
        SDL2-static SDL2main)