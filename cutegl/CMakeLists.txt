project(cutegl)

add_library(cutegl STATIC src/IPlatform.cpp)
target_include_directories(cutegl PUBLIC include)

if(UNIX)
    target_sources(cutegl PRIVATE src/X11Platform.cpp src/X11Platform.hpp)
endif()