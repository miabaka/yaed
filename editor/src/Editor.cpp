#include "Editor.hpp"

#include "game/TreasureHunterGame.hpp"
#include "importer/TreasureHunterWorldImporter.hpp"
#include "exporter/TreasureHunterWorldExporter.hpp"

Editor::Editor() {
	auto thGame = std::make_shared<TreasureHunterGame>();
	gameManager().registerGame(thGame);

	WorldFormat &thpFormat = worldFormatManager().registerFormat("sth_thp", {"Treasure Hunter Pack", "thp"});

	auto thpImporter = std::make_shared<TreasureHunterWorldImporter>();
	thpFormat.setImporter(thpImporter);

	auto thpExporter = std::make_shared<TreasureHunterWorldExporter>();
	thpFormat.setExporter(thpExporter);
}