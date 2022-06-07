#pragma once

namespace SthInternal {

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
}

namespace Skin {
enum {
	MummiesTreasures = 0,
	CyclopsOnGuard = 1,
	NorthernLegends = 2
};
};

} // namespace SthInternal