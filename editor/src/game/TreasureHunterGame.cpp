#include "TreasureHunterGame.hpp"

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
	Tile_TeleportIn1 = 13,
	Tile_TeleportIn2 = 14,
	Tile_TeleportIn3 = 15,
	Tile_TeleportIn4 = 16,
	Tile_TeleportIn5 = 17,
	Tile_TeleportIn6 = 18,
	Tile_TeleportIn7 = 19,
	Tile_TeleportIn8 = 20,
	Tile_TeleportIn9 = 21,
	Tile_TeleportInA = 22,
	Tile_TeleportInB = 23,
	Tile_TeleportInC = 24,
	Tile_TeleportInD = 25,
	Tile_TeleportInE = 26,

	Tile_TeleportOut0 = 27,
	Tile_TeleportOut1 = 28,
	Tile_TeleportOut2 = 29,
	Tile_TeleportOut3 = 30,
	Tile_TeleportOut4 = 31,
	Tile_TeleportOut5 = 32,
	Tile_TeleportOut6 = 33,
	Tile_TeleportOut7 = 34,
	Tile_TeleportOut8 = 35,
	Tile_TeleportOut9 = 36,
	Tile_TeleportOutA = 37,
	Tile_TeleportOutB = 38,
	Tile_TeleportOutC = 39,
	Tile_TeleportOutD = 40,
	Tile_TeleportOutE = 41,

	Tile_MonsterSpawn0 = 142,
	Tile_MonsterSpawn1 = 143,
	Tile_MonsterSpawn2 = 144,
	Tile_MonsterSpawn3 = 145,

	Tile_SecondMonsterSpawn0 = 146,
	Tile_SecondMonsterSpawn1 = 147,
	Tile_SecondMonsterSpawn2 = 148,
	Tile_SecondMonsterSpawn3 = 149,
	Tile_SecondMonsterSpawn4 = 150,
	Tile_SecondMonsterSpawn5 = 151,
	Tile_SecondMonsterSpawn6 = 152,
	Tile_SecondMonsterSpawn7 = 153,
	Tile_SecondMonsterSpawn8 = 154,
	Tile_SecondMonsterSpawn9 = 155,

	Tile_SecondMonsterRespawn0 = 156,

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