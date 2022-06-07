#include "SthPaletteIconProvider.hpp"

#include <array>

#include <GL/gl3w.h>

#define STB_IMAGE_IMPLEMENTATION

#define STBI_ONLY_PNG
#define STBI_NO_THREAD_LOCALS

#include "stb_image.h"
#include "SthInternal.hpp"

using namespace SthInternal;

// TODO: properly generate icon sets using tilemap renderer, brush definitions and some internal overrides
SthPaletteIconProvider::SthPaletteIconProvider() {
	int width, height, channelCount;
	void *pixelData = stbi_load("data/palette0.png", &width, &height, &channelCount, STBI_rgb_alpha);

	glGenTextures(1, &_testIconSetTexture);
	glBindTexture(GL_TEXTURE_2D, _testIconSetTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (pixelData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
		stbi_image_free(pixelData);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	void *tex = reinterpret_cast<void *>(_testIconSetTexture);

	_testIconSet = std::make_shared<PaletteIconSet>();

	const std::array<Tilemap::tile_t, 28> tiles = {
			Tile::Air,
			Tile::Hero,
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
			Tile::TeleportIn0,
			Tile::TeleportOut0,
			Tile::MonsterSpawn0,
			Tile::SecondMonsterSpawn0,
			Tile::SecondMonsterRespawn0,
			Tile::KeyItem,
			Tile::TrapItem,
			Tile::BombItem,
			Tile::FakeHeroItem,
			Tile::Gem0,
			Tile::Gem1,
			Tile::Gem2,
			Tile::Gem3,
			Tile::Gem4,
			Tile::Gem5,
			Tile::Bonus
	};

	int i = 0;

	for (Tilemap::tile_t tile: tiles) {
		auto x = (float) (i % 7);
		auto y = (float) (int) (i / 7);
		i++;
		_testIconSet->setIcon(tile, {tex, {x / 7, y / 4}, {(x + 1) / 7, (y + 1) / 4}});
	}
}

SthPaletteIconProvider::~SthPaletteIconProvider() {
	glDeleteTextures(1, &_testIconSetTexture);
}

const std::string &SthPaletteIconProvider::name() const {
	static std::string name = "SthPaletteIconProvider";
	return name;
}

std::set<std::string> SthPaletteIconProvider::supportedGameIds() const {
	return {"sth"};
}

std::shared_ptr<PaletteIconSet> SthPaletteIconProvider::getDefaultIconSetForWorld(const World &world) {
	return _testIconSet;
}

std::shared_ptr<PaletteIconSet> SthPaletteIconProvider::getIconSetForLevel(const Level &level) {
	return {};
}