project(cuteshell)

add_library(cuteshell STATIC
        src/CuteShell.cpp
        src/StubDialogProvider.cpp
        src/StubFileDialog.cpp)

if (WIN32)
    target_compile_definitions(cuteshell PRIVATE CUTE_DIALOGS_IMPL_WIN32)
    target_sources(cuteshell PRIVATE
            src/CuteShell_win32.cpp
            src/Win32DialogProvider.cpp
            src/Win32FileDialog.cpp)
elseif (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    pkg_check_modules(GLIB REQUIRED glibmm-2.4)
    pkg_check_modules(DBus REQUIRED dbus-1)

    target_include_directories(cuteshell PRIVATE ${GTK_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS} ${DBus_INCLUDE_DIRS})
    target_link_libraries(cuteshell PRIVATE ${GTK_LIBRARIES} ${GLIB_LIBRARIES} ${DBus_LIBRARIES})

    target_compile_definitions(cuteshell PRIVATE CUTE_DIALOGS_IMPL_GTK)
    target_sources(cuteshell PRIVATE
            src/CuteShell_nix.cpp
            src/GtkDialogProvider.cpp
            src/GtkFileDialog.cpp)
else ()
    target_sources(cuteshell PRIVATE src/CuteShell_unknown.cpp)
endif ()

target_include_directories(cuteshell PUBLIC include)