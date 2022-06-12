#pragma once

#include <filesystem>
#include <unordered_map>

#include <GL/gl3w.h>

#include "../../../core/Tilemap.hpp"
#include "AtlasEntry.hpp"
#include "InputAtlasEntry.hpp"

// TODO: proper cleanup
class AtlasPair {
public:
	enum class Tag {
		Common,
		Blocks,
		Knobs,
		LadderAndRope
	};

	enum class TextureSlot {
		Common,
		Alt
	};

	explicit AtlasPair(float defaultTileScale = 1.f);

	void loadTexture(const std::filesystem::path &path, TextureSlot slot);

	void appendEntries(const std::vector<InputAtlasEntry> &entries, glm::ivec2 atlasSize);

	void submitFrameOffsets();

	GLuint commonTexture() const;

	GLuint altTexture() const;

	GLuint frameOffsetTexture() const;

	float defaultTileScale() const;

	glm::vec2 commonScale() const;

	glm::vec2 altScale() const;

	const AtlasEntry &findEntryForTile(Tilemap::tile_t tile) const;

private:
	std::unordered_map<Tilemap::tile_t, AtlasEntry> _entries;
	std::vector<glm::vec2> _frameOffsets;
	glm::ivec2 _texCommonSize{};
	glm::ivec2 _texAltSize{};
	GLuint _texCommon{};
	GLuint _texAlt{};
	GLuint _texFrameOffsets{};
	float _defaultTileScale;
};