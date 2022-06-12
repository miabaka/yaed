#include "Editor.hpp"

#include "game/sth/SthGame.hpp"
#include "game/sth/SthPaletteIconProvider.hpp"
#include "game/sth/render/SthTilemapRenderer.hpp"
#include "game/sth/SthWorldExporter.hpp"
#include "game/sth/SthWorldFactory.hpp"
#include "game/sth/SthWorldImporter.hpp"

Editor::Editor() {
	auto sthGame = std::make_shared<SthGame>();
	games().registerGame(sthGame);

	auto sthWorldFactory = std::make_shared<SthWorldFactory>();
	worldFactories().registerFactory(sthWorldFactory);

	WorldFormat &thpFormat = worldFormats().registerFormat("sth_thp", {"Treasure Hunter Pack", "thp"});

	auto thpImporter = std::make_shared<SthWorldImporter>();
	thpFormat.setImporter(thpImporter);

	auto thpExporter = std::make_shared<SthWorldExporter>();
	thpFormat.setExporter(thpExporter);

	auto sthPaletteIconProvider = std::make_shared<SthPaletteIconProvider>();
	paletteIconProviders().registerProvider(sthPaletteIconProvider);

	auto sthTilemapRenderer = std::make_shared<SthTilemapRenderer>();
	tilemapRenderers().registerRenderer(sthTilemapRenderer);
}