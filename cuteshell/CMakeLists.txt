project(cuteshell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cuteshell STATIC
        include/cute/shell/CuteShell.hpp
        include/cute/shell/IDialogProvider.hpp
        include/cute/shell/IFileDialog.hpp
        src/CuteShell.cpp)

if (WIN32)
    target_compile_definitions(cuteshell PRIVATE CUTE_SHELL_WIN32_IMPL)
    target_sources(cuteshell PRIVATE
            src/win32/CuteShell.cpp
            src/win32/Win32DialogProvider.cpp
            src/win32/Win32DialogProvider.hpp
            src/win32/Win32FileDialog.cpp
            src/win32/Win32FileDialog.hpp)
else ()
    target_sources(cuteshell PRIVATE
            src/stub/CuteShell.cpp
            src/stub/StubDialogProvider.cpp
            src/stub/StubDialogProvider.hpp
            src/stub/StubFileDialog.cpp
            src/stub/StubFileDialog.hpp)
endif ()

target_include_directories(cuteshell PUBLIC include)