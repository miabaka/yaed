#include "AtlasPair.hpp"

#include <filesystem>

#include <stb/image.h>

#include "../SthInternal.hpp"
#include "InputAtlasEntry.hpp"

namespace fs = std::filesystem;

using namespace SthInternal;

static GLuint loadTexture(const fs::path &path) {
	int width, height, channelCount;
	void *pixelData = stbi_load(path.string().c_str(), &width, &height, &channelCount, STBI_rgb_alpha);

	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (pixelData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
		stbi_image_free(pixelData);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

AtlasPair::AtlasPair() {
	_texCommon = loadTexture("data/sth_renderer/textures/common.png");
	_texAlt = loadTexture("data/sth_renderer/textures/0_0.png");

	static const InputAtlasEntry commonEntries[] = {
			{{Tile::Bonus},        {0,    0},   {64, 64}, 32, 4},
			{{Tile::Hero},         {256,  0},   {64, 64}, 32, 4},
			{{Tile::BombItem},     {512,  0},   {64, 64}, 32, 4},
			{{Tile::FakeHeroItem}, {768,  0},   {64, 64}, 32, 4},
			{{Tile::KeyItem},      {1024, 0},   {64, 64}, 32, 4},
			{{Tile::TrapItem},     {1280, 0},   {64, 64}, 32, 4},
			{{},                   {0,    512}, {64, 64}, 32, 4},
			{{},                   {256,  512}, {64, 64}, 32, 4},
			{{},                   {512,  512}, {64, 64}, 32, 4},
			{{},                   {768,  512}, {64, 64}, 32, 4},
			{{},                   {1024, 512}, {64, 64}, 32, 4},
			{{},                   {1280, 512}, {64, 64}, 32, 4},
	};

	static const InputAtlasEntry altEntries[] = {
			// alt
			{{Tile::LockedExit}, {0,    0},   {64, 64}, 32, 4},
			{{Tile::HiddenExit}, {256,  0},   {64, 64}, 32, 4},
			{{Tile::Gem0},       {512,  0},   {64, 64}, 32, 4},
			{{Tile::Gem1},       {768,  0},   {64, 64}, 32, 4},
			{{Tile::Gem2},       {1024, 0},   {64, 64}, 32, 4},
			{{Tile::Gem3},       {0,    512}, {64, 64}, 32, 4},
			{{Tile::Gem4},       {256,  512}, {64, 64}, 32, 4},
			{{Tile::Gem5},       {512,  512}, {64, 64}, 32, 4},
			{{},                 {768,  512}, {64, 64}, 32, 4},
			{{},                 {1024, 512}, {64, 64}, 16, 4},
			{{},                 {1024, 768}, {64, 64}, 16, 4}
	};

	std::vector<glm::vec2> offsets;

	for (const auto &inputEntry: commonEntries) {
		for (Tilemap::tile_t tile: inputEntry.tiles) {
			AtlasEntry entry(static_cast<int>(offsets.size()), inputEntry.frameCount);
			_entries.insert({tile, entry});
		}

		inputEntry.computeOffsets(offsets, {1536, 1024});
	}

	for (const auto &inputEntry: altEntries) {
		for (Tilemap::tile_t tile: inputEntry.tiles) {
			AtlasEntry entry(static_cast<int>(offsets.size()), inputEntry.frameCount);
			_entries.insert({tile, entry});
		}

		inputEntry.computeOffsets(offsets, {1280, 1024});
	}

	glGenTextures(1, &_texFrameOffsets);

	glBindTexture(GL_TEXTURE_1D, _texFrameOffsets);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, static_cast<GLsizei>(offsets.size()), 0, GL_RG, GL_FLOAT, offsets.data());

	glBindTexture(GL_TEXTURE_1D, 0);
}

GLuint AtlasPair::commonTexture() const {
	return _texCommon;
}

GLuint AtlasPair::altTexture() const {
	return _texAlt;
}

GLuint AtlasPair::frameOffsetTexture() const {
	return _texFrameOffsets;
}

const AtlasEntry &AtlasPair::findEntryForTile(Tilemap::tile_t tile) const {
	const auto it = _entries.find(tile);

	if (it == _entries.end()) {
		static AtlasEntry stub(0);
		return stub;
	}

	return it->second;
}