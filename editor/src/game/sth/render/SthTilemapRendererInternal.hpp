#pragma once

#include <map>
#include <vector>

#include <GL/gl3w.h>

#include "../../../core/Level.hpp"
#include "AtlasPair.hpp"

class SthTilemapRendererInternal {
public:
	struct TileInstance {
		TileInstance(glm::ivec2 position, int material)
				: x(static_cast<GLshort>(position.x)),
				  y(static_cast<GLshort>(position.y)),
				  material(static_cast<GLshort>(material)) {}

		GLshort x;
		GLshort y;
		GLshort material;
	};

	SthTilemapRendererInternal();

	~SthTilemapRendererInternal();

	const AtlasPair &getAtlasPairFor(const Level &level, AtlasPair::Tag tag) const;

	void drawTiles(const AtlasPair &atlasPair, const std::vector<TileInstance> &tiles) const;

private:
	AtlasPair _commonAtlasPair;
	AtlasPair _ladderAndRopeAtlasPair;
	AtlasPair _knobAtlasPair;
	AtlasPair _blockAtlasPair;
	GLuint _program = 0;
	GLuint _vao = 0;
	GLuint _vbo = 0;

	void setupVbo();

	void setupVao();

	void setupProgram();

	void setupAtlasPairs();
};