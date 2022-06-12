#pragma once

#include <vector>

#include <GL/gl3w.h>

#include "../../../core/Level.hpp"
#include "AtlasPair.hpp"

class SthTilemapRendererInternal {
public:
	struct TileInstance {
		GLshort x;
		GLshort y;
		GLshort material;
	};

	SthTilemapRendererInternal();

	~SthTilemapRendererInternal();

	const AtlasPair &getAtlasPairFor(const Level &level) const;

	void drawTiles(const AtlasPair &atlasPair, const std::vector<TileInstance> &tiles) const;

private:
	GLuint _program = 0;
	GLuint _vao = 0;
	GLuint _vbo = 0;

	void setupVbo();

	void setupVao();

	void setupProgram();
};