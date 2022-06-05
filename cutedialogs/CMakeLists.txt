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
elseif(UNIX)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    pkg_check_modules(GLIB REQUIRED glibmm-2.4)

    target_include_directories(cutedialogs PRIVATE ${GTK_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS})
    target_link_libraries(cutedialogs PRIVATE ${GTK_LIBRARIES} ${GLIB_LIBRARIES})

    target_compile_definitions(cutedialogs PRIVATE CUTE_DIALOGS_IMPL_GTK)
    target_sources(cutedialogs PRIVATE
            src/GtkDialogProvider.cpp
            src/GtkFileDialog.cpp)
endif ()

target_include_directories(cutedialogs PUBLIC include)