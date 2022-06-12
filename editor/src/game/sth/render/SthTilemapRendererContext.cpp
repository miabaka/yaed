#include "SthTilemapRendererContext.hpp"

#include "../SthInternal.hpp"

using namespace SthInternal;

SthTilemapRendererContext::SthTilemapRendererContext(std::shared_ptr<SthTilemapRendererInternal> renderer)
		: _renderer(std::move(renderer)) {
	_texture = createTexture();
	_fbo = createFrameBuffer(_texture);
}

SthTilemapRendererContext::~SthTilemapRendererContext() {
	glDeleteFramebuffers(1, &_fbo);
	glDeleteTextures(1, &_texture);
}

glm::uvec2 SthTilemapRendererContext::tileSize() const {
	return {40, 40};
}

glm::uvec2 SthTilemapRendererContext::viewportSize() const {
	return {1600, 1200};
}

void *SthTilemapRendererContext::viewportTexture() const {
	return reinterpret_cast<void *>(_texture);
}

bool SthTilemapRendererContext::compatibleWith(const IGame &game) {
	return false;
}

void SthTilemapRendererContext::setLevel(std::weak_ptr<Level> level) {
	_level = std::move(level);
}

void SthTilemapRendererContext::render() {
	if (_level.expired())
		return;

	std::shared_ptr<Level> level = _level.lock();

	const Tilemap &gemLayerMap = level->layers()[0]->tilemap();
	const Tilemap &mainLayerMap = level->layers()[1]->tilemap();

	glm::ivec2 size = gemLayerMap.size();

	const AtlasPair &atlasPair = _renderer->getAtlasPairFor(*level);

	std::vector<SthTilemapRendererInternal::TileInstance> tiles;

	for (int i = 0; i < gemLayerMap.tileCount(); i++) {
		glm::ivec2 position = {i % size.x, i / size.x};

		Tilemap::tile_t tile = gemLayerMap(position);

		if (tile < Tile::Gem0 || tile > Tile::Gem5)
			continue;

		int frame = atlasPair.findEntryForTile(tile).firstFrame();

		tiles.push_back({static_cast<GLshort>(position.x), static_cast<GLshort>(position.y), static_cast<GLshort>(frame)});
	}

	GLint previousFbo;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousFbo);

	glViewport(0, 0, 1600, 1200);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_renderer->drawTiles(atlasPair, tiles);

	glDisable(GL_BLEND);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, previousFbo);
}

GLuint SthTilemapRendererContext::createTexture() {
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1600, 1200, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint SthTilemapRendererContext::createFrameBuffer(GLuint texture) {
	GLuint fbo;
	glGenFramebuffers(1, &fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}