project(cutegl)

add_library(cutegl STATIC src/IPlatform.cpp include/IWindow.hpp)
target_include_directories(cutegl PUBLIC include)

if(UNIX)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(X11 REQUIRED x11 glx)

    target_sources(cutegl PRIVATE src/X11Platform.cpp src/X11Platform.hpp)
    target_include_directories(cutegl PRIVATE ${X11_INCLUDE_DIRS})
    target_link_libraries(cutegl PRIVATE ${X11_LIBRARIES})
endif()