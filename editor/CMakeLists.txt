project(editor)

find_package(glm REQUIRED)

add_executable(editor
        src/core/game/GameManager.cpp
        src/core/game/KnownGame.cpp
        src/core/game/UnknownGame.cpp
        src/core/BaseEditor.cpp
        src/core/BaseWorldExporter.cpp
        src/core/Layer.cpp
        src/core/LayerTemplate.cpp
        src/core/Level.cpp
        src/core/Tilemap.cpp
        src/core/World.cpp
        src/core/WorldManager.cpp
        src/core/format/WorldFormat.cpp
        src/core/format/WorldFormatInfo.cpp
        src/core/format/WorldFormatManager.cpp
        src/exporters/TreasureHunterWorldExporter.cpp
        src/games/TreasureHunterGame.cpp
        src/importers/TreasureHunterWorldImporter.cpp
        src/Editor.cpp
        src/main.cpp)