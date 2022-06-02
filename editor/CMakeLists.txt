project(editor)

find_package(glm REQUIRED)

add_executable(editor
        src/core/games/GameManager.cpp
        src/core/games/KnownGame.cpp
        src/core/games/UnknownGame.cpp
        src/core/BaseEditor.cpp
        src/core/BaseWorldExporter.cpp
        src/core/Layer.cpp
        src/core/LayerTemplate.cpp
        src/core/Level.cpp
        src/core/Tilemap.cpp
        src/core/World.cpp
        src/core/formats/WorldFormat.cpp
        src/core/formats/WorldFormatInfo.cpp
        src/core/formats/WorldFormatManager.cpp
        src/exporters/TreasureHunterWorldExporter.cpp
        src/games/TreasureHunterGame.cpp
        src/importers/TreasureHunterWorldImporter.cpp
        src/Editor.cpp
        src/main.cpp)