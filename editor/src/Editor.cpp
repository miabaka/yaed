#include "Editor.hpp"

#include "games/TreasureHunterGame.hpp"
#include "importers/TreasureHunterWorldImporter.hpp"

Editor::Editor() {
	auto thGame = std::make_shared<TreasureHunterGame>();
	registerGame(thGame);

	WorldFormat &thpFormat = registerWorldFormat("sth_thp", {"Treasure Hunter Pack", "thp"});

	auto thpImporter = std::make_shared<TreasureHunterWorldImporter>();
	thpFormat.registerImporter(thpImporter);
}