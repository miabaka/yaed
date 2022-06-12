#pragma once

#include <unordered_map>

#include <GL/gl3w.h>

#include "../../../core/Tilemap.hpp"
#include "AtlasEntry.hpp"

class AtlasPair {
public:
	AtlasPair();

	GLuint commonTexture() const;

	GLuint altTexture() const;

	GLuint frameOffsetTexture() const;

	const AtlasEntry &findEntryForTile(Tilemap::tile_t tile) const;

private:
	std::unordered_map<Tilemap::tile_t, AtlasEntry> _entries;
	GLuint _texCommon{};
	GLuint _texAlt{};
	GLuint _texFrameOffsets{};
};