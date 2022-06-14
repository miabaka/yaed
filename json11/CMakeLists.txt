project(json11)

add_library(json11 STATIC src/json11.cpp)

target_include_directories(json11 PUBLIC include)