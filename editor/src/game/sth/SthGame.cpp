#include "SthGame.hpp"

#include "SthInternal.hpp"
#include "../../core/palette/PaletteTemplateBuilder.hpp"

using namespace SthInternal;

SthGame::SthGame()
		: KnownGame("sth", "Snowy: Treasure Hunter") {
	setupLayerTemplates();
	setupLevelSkins();
	setupPaletteTemplate();
}

void SthGame::setupLayerTemplates() {
	std::unordered_set<Tilemap::tile_t> mainTiles = {
			Tile::Air,
			Tile::Ground,
			Tile::AltGround,
			Tile::Concrete,
			Tile::Ice,
			Tile::Hole,
			Tile::Ladder,
			Tile::HiddenLadder,
			Tile::Rope,
			Tile::HiddenExit,
			Tile::LockedExit,
			Tile::Hero,
			Tile::TeleportIn0,
			Tile::TeleportOut0,
			Tile::MonsterSpawn0,
			Tile::SecondMonsterSpawn0,
			Tile::SecondMonsterRespawn0,
			Tile::KeyItem,
			Tile::TrapItem,
			Tile::BombItem,
			Tile::FakeHeroItem,
			Tile::Bonus
	};

	std::unordered_set<Tilemap::tile_t> gemTiles = {
			Tile::Air,
			Tile::Gem0,
			Tile::Gem1,
			Tile::Gem2,
			Tile::Gem3,
			Tile::Gem4,
			Tile::Gem5
	};

	auto mainLayer = std::make_shared<LayerTemplate>("Main", std::move(mainTiles));

	mainLayer->setDefaultPrimaryTile(Tile::Ground);
	mainLayer->setDefaultSecondaryTile(Tile::Air);

	addLayerTemplate("main", mainLayer);

	auto gemLayer = std::make_shared<LayerTemplate>("Gems", std::move(gemTiles));

	gemLayer->setDefaultPrimaryTile(Tile::Gem0);
	gemLayer->setDefaultSecondaryTile(Tile::Air);

	addLayerTemplate("gems", gemLayer);
}

void SthGame::setupLevelSkins() {
	addLevelSkin(std::make_shared<LevelSkin>(Skin::MummiesTreasures, "Mummies' Treasures"));
	addLevelSkin(std::make_shared<LevelSkin>(Skin::CyclopsOnGuard, "Cyclops on Guard!"));
	addLevelSkin(std::make_shared<LevelSkin>(Skin::NorthernLegends, "Northern Legends"));
}

void SthGame::setupPaletteTemplate() {
	std::shared_ptr<PaletteTemplate> paletteTemplate = PaletteTemplate::builder()
			.beginGroup("Common")
			.brush("Air", Tile::Air)
			.brush("Hero", Tile::Hero)
			.brush("Bonus", Tile::Bonus)
			.submitGroup()
			.beginGroup("Blocks")
			.brush("Ground", Tile::Ground)
			.brush("Alt Ground", Tile::AltGround)
			.brush("Concrete", Tile::Concrete)
			.brush("Ice", Tile::Ice)
			.brush("Hole", Tile::Hole)
			.brush("Ladder", Tile::Ladder)
			.brush("Hidden Ladder", Tile::HiddenLadder)
			.brush("Rope", Tile::Rope)
			.brush("Hidden Exit", Tile::HiddenExit)
			.brush("Locked Exit", Tile::LockedExit)
			.submitGroup()
			.beginGroup("Objects")
			.brush("Teleport In", {Tile::TeleportIn0, Tile::TeleportInE})
			.brush("Teleport Out", {Tile::TeleportOut0, Tile::TeleportOutE})
			.submitGroup()
			.beginGroup("Monsters")
			.brush("Monster", {Tile::MonsterSpawn0, Tile::MonsterSpawn3})
			.brush("Monster 2", {Tile::SecondMonsterSpawn0, Tile::SecondMonsterSpawn9})
			.brush("Monster 2 Respawn", {Tile::SecondMonsterRespawn0, Tile::SecondMonsterRespawn9})
			.submitGroup()
			.beginGroup("Items")
			.brush("Key", Tile::KeyItem)
			.brush("Trap", Tile::TrapItem)
			.brush("Bomb", Tile::BombItem)
			.brush("Fake Hero", Tile::FakeHeroItem)
			.submitGroup()
			.beginGroup("Gems")
			.brush("Gem", Tile::Gem0)
			.brush("Gem 2", Tile::Gem1)
			.brush("Gem 3", Tile::Gem2)
			.brush("Gem 4", Tile::Gem3)
			.brush("Gem 5", Tile::Gem4)
			.brush("Gem 6", Tile::Gem5)
			.submitGroup()
			.collect();

	setPaletteTemplate(paletteTemplate);
}