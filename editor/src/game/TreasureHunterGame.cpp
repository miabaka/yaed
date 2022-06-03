#include "TreasureHunterGame.hpp"

#include "../core/PaletteTemplateBuilder.hpp"

enum Tile {
	Tile_Air = 0,

	Tile_Ground = 1,
	Tile_AltGround = 2,
	Tile_Concrete = 3,
	Tile_Ice = 4,
	Tile_Hole = 5,
	Tile_Ladder = 6,
	Tile_HiddenLadder = 7,
	Tile_Rope = 8,
	Tile_HiddenExit = 9,
	Tile_LockedExit = 10,

	Tile_Hero = 11,

	Tile_TeleportIn0 = 12,
	Tile_TeleportInE = 26,

	Tile_TeleportOut0 = 27,
	Tile_TeleportOutE = 41,

	Tile_MonsterSpawn0 = 142,
	Tile_MonsterSpawn3 = 145,

	Tile_SecondMonsterSpawn0 = 146,
	Tile_SecondMonsterSpawn9 = 155,

	Tile_SecondMonsterRespawn0 = 156,
	Tile_SecondMonsterRespawn9 = 165,

	Tile_KeyItem = 268,
	Tile_TrapItem = 269,
	Tile_BombItem = 271,
	TIle_FakeHeroItem = 272,

	Tile_Gem0 = 373,
	Tile_Gem1 = 374,
	Tile_Gem2 = 375,
	Tile_Gem3 = 376,
	Tile_Gem4 = 377,
	Tile_Gem5 = 378,

	Tile_Bonus = 483
};

TreasureHunterGame::TreasureHunterGame()
		: KnownGame("sth", "Snowy: Treasure Hunter") {
	setupLayerTemplates();
	setupPaletteTemplate();
}

void TreasureHunterGame::setupLayerTemplates() {
	std::set<Tilemap::tile_t> mainTiles = {
			Tile_Air,
			Tile_Ground,
			Tile_AltGround,
			Tile_Concrete,
			Tile_Ice,
			Tile_Hole,
			Tile_Ladder,
			Tile_HiddenLadder,
			Tile_Rope,
			Tile_HiddenExit,
			Tile_LockedExit,
			Tile_Hero,
			Tile_TeleportIn0,
			Tile_TeleportOut0,
			Tile_MonsterSpawn0,
			Tile_SecondMonsterSpawn0,
			Tile_SecondMonsterRespawn0,
			Tile_KeyItem,
			Tile_TrapItem,
			Tile_BombItem,
			TIle_FakeHeroItem,
			Tile_Bonus
	};

	std::set<Tilemap::tile_t> gemTiles = {
			Tile_Air,
			Tile_Gem0,
			Tile_Gem1,
			Tile_Gem2,
			Tile_Gem3,
			Tile_Gem4,
			Tile_Gem5
	};

	addLayerTemplate("main", std::make_shared<LayerTemplate>("Main", std::move(mainTiles)));
	addLayerTemplate("gems", std::make_shared<LayerTemplate>("Gems", std::move(gemTiles)));
}

void TreasureHunterGame::setupPaletteTemplate() {
	std::shared_ptr<PaletteTemplate> paletteTemplate = PaletteTemplate::builder()
			.beginGroup("Common")
			.brush("Air", Tile_Air)
			.brush("Hero", Tile_Hero)
			.brush("Bonus", Tile_Bonus)
			.submitGroup()
			.beginGroup("Blocks")
			.brush("Ground", Tile_Ground)
			.brush("Alt Ground", Tile_AltGround)
			.brush("Concrete", Tile_Concrete)
			.brush("Ice", Tile_Ice)
			.brush("Hole", Tile_Hole)
			.brush("Ladder", Tile_Ladder)
			.brush("Hidden Ladder", Tile_HiddenLadder)
			.brush("Rope", Tile_Rope)
			.brush("Hidden Exit", Tile_HiddenExit)
			.brush("Locked Exit", Tile_LockedExit)
			.submitGroup()
			.beginGroup("Objects")
			.brush("Teleport In", {Tile_TeleportIn0, Tile_TeleportInE})
			.brush("Teleport Out", {Tile_TeleportOut0, Tile_TeleportOutE})
			.submitGroup()
			.beginGroup("Monsters")
			.brush("Monster", {Tile_MonsterSpawn0, Tile_MonsterSpawn3})
			.brush("Monster 2", {Tile_SecondMonsterSpawn0, Tile_SecondMonsterSpawn9})
			.brush("Monster 2 Respawn", {Tile_SecondMonsterRespawn0, Tile_SecondMonsterRespawn9})
			.submitGroup()
			.beginGroup("Items")
			.brush("Key", Tile_KeyItem)
			.brush("Trap", Tile_TrapItem)
			.brush("Bomb", Tile_BombItem)
			.brush("Fake Hero", TIle_FakeHeroItem)
			.submitGroup()
			.beginGroup("Gems")
			.brush("Gem", Tile_Gem0)
			.brush("Gem 2", Tile_Gem1)
			.brush("Gem 3", Tile_Gem2)
			.brush("Gem 4", Tile_Gem3)
			.brush("Gem 5", Tile_Gem4)
			.brush("Gem 6", Tile_Gem5)
			.submitGroup()
			.collect();
}