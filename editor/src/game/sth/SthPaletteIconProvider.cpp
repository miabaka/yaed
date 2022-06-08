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
	loadIconSet("data/fallback_palette_icons/sth_0.png", Skin::MummiesTreasures);
	loadIconSet("data/fallback_palette_icons/sth_1.png", Skin::CyclopsOnGuard);
	loadIconSet("data/fallback_palette_icons/sth_2.png", Skin::NorthernLegends);
}

SthPaletteIconProvider::~SthPaletteIconProvider() {
	for (auto handle: _textureHandles)
		glDeleteTextures(1, &handle);
}

const std::string &SthPaletteIconProvider::name() const {
	static std::string name = "SthPaletteIconProvider";
	return name;
}

std::set<std::string> SthPaletteIconProvider::supportedGameIds() const {
	return {"sth"};
}

std::shared_ptr<PaletteIconSet> SthPaletteIconProvider::getDefaultIconSetForWorld(const World &world) {
	return {};
}

std::shared_ptr<PaletteIconSet> SthPaletteIconProvider::getIconSetForLevel(const Level &level) {
	std::shared_ptr<LevelSkin> skin = level.skin();

	if (!skin)
		return {};

	const auto it = _iconSets.find(skin->id());

	if (it == _iconSets.end())
		return {};

	return it->second;
}

void SthPaletteIconProvider::loadIconSet(const std::filesystem::path &path, int id) {
	GLuint texture;

	int width, height, channelCount;
	void *pixelData = stbi_load(path.string().c_str(), &width, &height, &channelCount, STBI_rgb_alpha);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

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

	static const std::array<Tilemap::tile_t, 28> tiles = {
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

	auto iconSet = std::make_shared<PaletteIconSet>();

	int i = 0;

	for (Tilemap::tile_t tile: tiles) {
		auto x = (float) (i % 7);
		auto y = (float) (int) (i / 7);
		i++;
		iconSet->setIcon(tile, {reinterpret_cast<void *>(texture), {x / 7, y / 4}, {(x + 1) / 7, (y + 1) / 4}});
	}

	_textureHandles.insert(texture);
	_iconSets.insert({id, iconSet});
}