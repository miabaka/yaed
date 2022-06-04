project(cutedialogs)

add_library(cutedialogs STATIC
        src/CuteDialogs.cpp
        src/StubDialogProvider.cpp
        src/StubFileDialog.cpp)

if (WIN32)
    target_compile_definitions(cutedialogs PRIVATE CUTE_DIALOGS_IMPL_WIN32)
    target_sources(cutedialogs PRIVATE
            src/Win32DialogProvider.cpp
            src/Win32FileDialog.cpp)
endif ()

target_include_directories(cutedialogs PUBLIC include)