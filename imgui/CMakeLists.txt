project(imgui)

add_library(imgui STATIC
        src/imgui_impl_glfw.cpp
        src/imgui_impl_opengl3.cpp
        src/imgui.cpp
        src/imgui_demo.cpp
        src/imgui_draw.cpp
        src/imgui_stdlib.cpp
        src/imgui_tables.cpp
        src/imgui_widgets.cpp)

target_include_directories(imgui PUBLIC include)

target_link_libraries(imgui PRIVATE glfw gl3w)