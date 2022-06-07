#include "Editor.hpp"

#include "game/sth/SthGame.hpp"
#include "game/sth/SthPaletteIconProvider.hpp"
#include "game/sth/SthWorldExporter.hpp"
#include "game/sth/SthWorldImporter.hpp"
#include "game/sth/SthWorldFactory.hpp"

Editor::Editor() {
	auto thGame = std::make_shared<SthGame>();
	gameManager().registerGame(thGame);

	auto thWorldFactory = std::make_shared<SthWorldFactory>();
	worldFactories().registerFactory(thWorldFactory);

	WorldFormat &thpFormat = worldFormats().registerFormat("sth_thp", {"Treasure Hunter Pack", "thp"});

	auto thpImporter = std::make_shared<SthWorldImporter>();
	thpFormat.setImporter(thpImporter);

	auto thpExporter = std::make_shared<SthWorldExporter>();
	thpFormat.setExporter(thpExporter);

	auto thPaletteIconProvider = std::make_shared<SthPaletteIconProvider>();
	paletteIconProviders().registerProvider(thPaletteIconProvider);
}