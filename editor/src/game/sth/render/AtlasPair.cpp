#include "AtlasPair.hpp"

#include <stb/image.h>

namespace fs = std::filesystem;

AtlasPair::AtlasPair(float defaultTileScale)
		: _defaultTileScale(defaultTileScale) {}

static GLuint loadTexture(const fs::path &path, glm::ivec2 &outSize) {
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

	outSize.x = width;
	outSize.y = height;

	return texture;
}

void AtlasPair::loadTexture(const std::filesystem::path &path, TextureSlot slot) {
	switch (slot) {
		case TextureSlot::Common:
			_texCommon = ::loadTexture(path, _texCommonSize);
			break;

		case TextureSlot::Alt:
			_texAlt = ::loadTexture(path, _texAltSize);

		default:
			break;
	}
}

void AtlasPair::appendEntries(const std::vector<InputAtlasEntry> &entries, glm::ivec2 atlasSize) {
	for (const auto &inputEntry: entries) {
		const auto firstFrame = static_cast<int>(_frameOffsets.size());

		for (Tilemap::tile_t tile: inputEntry.tiles) {
			AtlasEntry entry(firstFrame, inputEntry.frameCount);
			_entries.insert({tile, entry});
		}

		inputEntry.computeOffsets(_frameOffsets, atlasSize);
	}
}

void AtlasPair::submitFrameOffsets() {
	glGenTextures(1, &_texFrameOffsets);

	glBindTexture(GL_TEXTURE_1D, _texFrameOffsets);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	const auto count = static_cast<GLsizei>(_frameOffsets.size());
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, count, 0, GL_RG, GL_FLOAT, _frameOffsets.data());

	glBindTexture(GL_TEXTURE_1D, 0);

	_frameOffsets.clear();
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

float AtlasPair::defaultTileScale() const {
	return _defaultTileScale;
}

glm::vec2 AtlasPair::commonScale() const {
	return 40 * defaultTileScale() / glm::vec2(_texCommonSize);
}

glm::vec2 AtlasPair::altScale() const {
	return 40 * defaultTileScale() / glm::vec2(_texAltSize);
}

const AtlasEntry &AtlasPair::findEntryForTile(Tilemap::tile_t tile) const {
	const auto it = _entries.find(tile);

	if (it == _entries.end()) {
		static AtlasEntry stub(0);
		return stub;
	}

	return it->second;
}