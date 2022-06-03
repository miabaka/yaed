project(cutegl)

add_library(cutegl STATIC src/IPlatform.cpp)
target_include_directories(cutegl PUBLIC include)
