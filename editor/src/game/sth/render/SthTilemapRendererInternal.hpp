#pragma once

#include <unordered_map>
#include <vector>

#include <GL/gl3w.h>

#include "../../../core/Level.hpp"
#include "Atlas.hpp"

class SthTilemapRendererInternal {
public:
	struct TileInstance {
		TileInstance(glm::ivec2 position, int material)
				: x(static_cast<GLshort>(position.x)),
				  y(static_cast<GLshort>(position.y)),
				  material(static_cast<GLshort>(material)) {}

		GLushort x;
		GLushort y;
		GLushort material;
	};

	SthTilemapRendererInternal();

	~SthTilemapRendererInternal();

	const Atlas &getAtlas();

	void drawTiles(const std::vector<TileInstance> &tiles, float opacity = 1.f) const;

	void selectAtlasForSkin(int skinId);

private:
	Atlas _atlas;
	std::unordered_map<int, GLuint> _atlasTextures;
	GLuint _program = 0;
	GLuint _vao = 0;
	GLuint _vbo = 0;
	GLuint _texFrameOffsets = 0;
	GLuint _texAtlas = 0;

	void setupVbo();

	void setupVao();

	void setupProgram();

	void loadAtlas();
};