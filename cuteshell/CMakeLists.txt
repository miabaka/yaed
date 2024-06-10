project(cuteshell)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cuteshell STATIC
        include/cute/shell/IDialogProvider.hpp
        include/cute/shell/IFileDialog.hpp
        src/stub/StubDialogProvider.hpp
        src/stub/StubDialogProvider.cpp
        src/stub/StubFileDialog.cpp
        src/stub/StubFileDialog.hpp
        src/CuteShell.cpp)

if (MSVC)
    target_compile_options(cuteshell PRIVATE /Zc:char8_t-)
else ()
    target_compile_options(cuteshell PRIVATE -fno-char8_t)
endif ()

if (WIN32)
    target_compile_definitions(cuteshell PRIVATE CUTE_SHELL_WIN32_DIALOGS)

    target_sources(cuteshell PRIVATE
            src/win32/CuteShell.cpp
            src/win32/Win32DialogProvider.cpp
            src/win32/Win32DialogProvider.hpp
            src/win32/Win32FileDialog.cpp
            src/win32/Win32FileDialog.hpp)
elseif (UNIX AND NOT APPLE)
    find_package(sdbus-c++ REQUIRED)

    target_compile_definitions(cuteshell PRIVATE CUTE_SHELL_XDG_DIALOGS)

    target_sources(cuteshell PRIVATE
            src/linux/CuteShell.cpp
            src/linux/XdgDialogProvider.cpp
            src/linux/XdgDialogProvider.hpp
            src/linux/XdgFileDialog.cpp
            src/linux/XdgFileDialog.hpp)

    target_link_libraries(cuteshell PRIVATE sdbus-c++)
else ()
    target_sources(cuteshell PRIVATE
            src/stub/CuteShell.cpp)
endif ()

target_include_directories(cuteshell PUBLIC include)

target_link_libraries(cuteshell PRIVATE fmt)