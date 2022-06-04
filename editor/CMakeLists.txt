project(editor)

add_executable(editor
        src/core/format/WorldFormat.cpp
        src/core/format/WorldFormatInfo.cpp
        src/core/format/WorldFormatManager.cpp
        src/core/game/GameManager.cpp
        src/core/game/KnownGame.cpp
        src/core/game/UnknownGame.cpp
        src/core/BaseEditor.cpp
        src/core/BaseWorldExporter.cpp
        src/core/Brush.cpp
        src/core/BrushGroup.cpp
        src/core/CustomDataContainer.cpp
        src/core/Layer.cpp
        src/core/LayerTemplate.cpp
        src/core/Level.cpp
        src/core/PaletteTemplate.cpp
        src/core/PaletteTemplateBuilder.cpp
        src/core/Tilemap.cpp
        src/core/World.cpp
        src/core/WorldFactoryManager.cpp
        src/core/WorldManager.cpp
        src/exporter/TreasureHunterWorldExporter.cpp
        src/game/TreasureHunterGame.cpp
        src/importer/TreasureHunterWorldImporter.cpp
        src/world_factory/TreasureHunterWorldFactory.cpp
        src/Editor.cpp
        src/EditorApplication.cpp
        src/main.cpp)

target_link_libraries(editor PRIVATE glm glfw gl3w imgui)