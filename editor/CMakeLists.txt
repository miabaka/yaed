project(editor)

find_package(glm REQUIRED)

add_executable(editor
        src/core/BaseEditor.cpp
        src/core/KnownGame.cpp
        src/core/Layer.cpp
        src/core/LayerTemplate.cpp
        src/core/Level.cpp
        src/core/Tilemap.cpp
        src/core/UnknownGame.cpp
        src/core/World.cpp
        src/core/WorldFormat.cpp
        src/core/WorldFormatInfo.cpp
        src/games/TreasureHunterGame.cpp
        src/importers/TreasureHunterWorldImporter.cpp
        src/Editor.cpp
        src/main.cpp)