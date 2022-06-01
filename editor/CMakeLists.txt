project(editor)

find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(editor
        src/importers/TreasureHunterWorldImporter.cpp
        src/Editor.cpp
        src/Game.cpp
        src/Layer.cpp
        src/LayerTemplate.cpp
        src/Level.cpp
        src/main.cpp
        src/Tilemap.cpp
        src/World.cpp
        src/WorldFormat.cpp
        src/WorldFormatInfo.cpp)