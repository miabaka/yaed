project(cutedialogs)

add_library(cutedialogs STATIC
        src/core.cpp
        src/Win32DialogProvider.cpp
        src/Win32FileDialog.cpp)

target_include_directories(cutedialogs PUBLIC include)