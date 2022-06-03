#include "TreasureHunterGame.hpp"

#include "../core/PaletteTemplateBuilder.hpp"

namespace Tile {

enum {
	Air = 0,

	Ground = 1,
	AltGround = 2,
	Concrete = 3,
	Ice = 4,
	Hole = 5,
	Ladder = 6,
	HiddenLadder = 7,
	Rope = 8,
	HiddenExit = 9,
	LockedExit = 10,

	Hero = 11,

	TeleportIn0 = 12,
	TeleportInE = 26,

	TeleportOut0 = 27,
	TeleportOutE = 41,

	MonsterSpawn0 = 142,
	MonsterSpawn3 = 145,

	SecondMonsterSpawn0 = 146,
	SecondMonsterSpawn9 = 155,

	SecondMonsterRespawn0 = 156,
	SecondMonsterRespawn9 = 165,

	KeyItem = 268,
	TrapItem = 269,
	BombItem = 271,
	FakeHeroItem = 272,

	Gem0 = 373,
	Gem1 = 374,
	Gem2 = 375,
	Gem3 = 376,
	Gem4 = 377,
	Gem5 = 378,

	Bonus = 483
};

} // namespace Tile

TreasureHunterGame::TreasureHunterGame()
		: KnownGame("sth", "Snowy: Treasure Hunter") {
	setupLayerTemplates();
	setupPaletteTemplate();
}

void TreasureHunterGame::setupLayerTemplates() {
	std::set<Tilemap::tile_t> mainTiles = {
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

	std::set<Tilemap::tile_t> gemTiles = {
			Tile::Air,
			Tile::Gem0,
			Tile::Gem1,
			Tile::Gem2,
			Tile::Gem3,
			Tile::Gem4,
			Tile::Gem5
	};

	addLayerTemplate("main", std::make_shared<LayerTemplate>("Main", std::move(mainTiles)));
	addLayerTemplate("gems", std::make_shared<LayerTemplate>("Gems", std::move(gemTiles)));
}

void TreasureHunterGame::setupPaletteTemplate() {
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