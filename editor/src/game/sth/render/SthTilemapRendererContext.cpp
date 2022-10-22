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

glm::ivec2 SthTilemapRendererContext::tileSize() const {
	return {40, 40};
}

glm::ivec2 SthTilemapRendererContext::viewportSize() const {
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

static inline bool isBlockTile(Tilemap::tile_t tile) {
	return tile >= Tile::Ground && tile <= Tile::Hole;
}

static inline bool isLadderTile(Tilemap::tile_t tile) {
	return tile == Tile::Ladder || tile == Tile::HiddenLadder;
}

static inline bool isSolidTile(Tilemap::tile_t tile) {
	return isBlockTile(tile) || isLadderTile(tile);
}

static inline bool isMonsterTile(Tilemap::tile_t tile) {
	return tile >= Tile::MonsterSpawn0 && tile <= Tile::MonsterSpawn3;
}

static inline bool isSecondMonsterTile(Tilemap::tile_t tile) {
	return (tile >= Tile::SecondMonsterSpawn0 && tile <= Tile::SecondMonsterSpawn9)
	       || (tile >= Tile::SecondMonsterRespawn0 && tile <= Tile::SecondMonsterRespawn9);
}

static inline bool isAnyMonsterTile(Tilemap::tile_t tile) {
	return isMonsterTile(tile) || isSecondMonsterTile(tile);
}

static inline bool isAnyTeleportTile(Tilemap::tile_t tile) {
	return (tile >= Tile::TeleportIn0 && tile <= Tile::TeleportInE)
	       || (tile >= Tile::TeleportOut0 && tile <= Tile::TeleportOutE)
	       || tile == Tile::HiddenExit
	       || tile == Tile::LockedExit;
}

static inline bool isGemTile(Tilemap::tile_t tile) {
	return tile >= Tile::Gem0 && tile <= Tile::Gem5;
}

static inline bool isItemTile(Tilemap::tile_t tile) {
	return tile == Tile::KeyItem || tile == Tile::TrapItem || tile == Tile::BombItem || tile == Tile::FakeHeroItem;
}

void SthTilemapRendererContext::render() {
	if (_level.expired())
		return;

	std::shared_ptr<Level> level = _level.lock();

	const Tilemap &gemLayerMap = level->layers()[0]->tilemap();
	const Tilemap &mainLayerMap = level->layers()[1]->tilemap();

	if (gemLayerMap.size() != mainLayerMap.size())
		return;

	glm::ivec2 size = gemLayerMap.size();

	const Atlas &atlas = _renderer->getAtlas();

	// TODO: use preallocated memory
	std::vector<SthTilemapRendererInternal::TileInstance> tiles;

	tiles.reserve(4800);

	{
		const uint16_t ropeKnobMaterial = atlas.findEntryForTile(502).firstFrame();
		const uint16_t ladderKnobMaterial = atlas.findEntryForTile(503).firstFrame();

		for (int i = 0; i < mainLayerMap.tileCount(); i++) {
			const glm::ivec2 position = {i % size.x, i / size.x};

			const Tilemap::tile_t tile = mainLayerMap(position);

			if (!(isLadderTile(tile) || tile == Tile::Rope))
				continue;

			const uint16_t material = atlas.findEntryForTile(tile).firstFrame();

			tiles.emplace_back(position, material);

			if (tile == Tile::Rope) {
				const Tilemap::tile_t leftTile = mainLayerMap(position + glm::ivec2(-1, 0));
				const Tilemap::tile_t rightTile = mainLayerMap(position + glm::ivec2(1, 0));

				const bool hasLeftNeighbor = (leftTile == Tile::Rope);
				const bool hasRightNeighbor = (rightTile == Tile::Rope);

				if (hasLeftNeighbor && hasRightNeighbor)
					continue;

				tiles.emplace_back(position, ropeKnobMaterial + ((hasLeftNeighbor << 1) | hasRightNeighbor));
			} else {
				const Tilemap::tile_t topTile = mainLayerMap(position + glm::ivec2(0, -1));
				const Tilemap::tile_t bottomTile = mainLayerMap(position + glm::ivec2(0, 1));

				const bool hasTopNeighbor = isLadderTile(topTile);
				const bool hasBottomNeighbor = isLadderTile(bottomTile);

				if (hasTopNeighbor && hasBottomNeighbor)
					continue;

				tiles.emplace_back(position, ladderKnobMaterial + ((hasTopNeighbor << 1) | hasBottomNeighbor));
			}
		}
	}

	{
		const uint16_t &pointerMaterial = atlas.findEntryForTile(500).firstFrame();

		for (int i = 0; i < mainLayerMap.tileCount(); i++) {
			const glm::ivec2 position = {i % size.x, i / size.x};

			const Tilemap::tile_t tile = mainLayerMap(position);

			if (isSolidTile(tile))
				continue;

			const Tilemap::tile_t bottomMainTile = mainLayerMap(position + glm::ivec2{0, 1});
			const Tilemap::tile_t bottomGemTile = gemLayerMap(position + glm::ivec2{0, 1});

			if (!(isBlockTile(bottomMainTile) && isGemTile(bottomGemTile)))
				continue;

			tiles.emplace_back(position, pointerMaterial);
		}
	}

	for (int i = 0; i < gemLayerMap.tileCount(); i++) {
		const glm::ivec2 position = {i % size.x, i / size.x};

		const Tilemap::tile_t tile = gemLayerMap(position);

		if (!isGemTile(tile))
			continue;

		const uint16_t material = atlas.findEntryForTile(tile).firstFrame();

		tiles.emplace_back(position, material);
	}

	for (int i = 0; i < mainLayerMap.tileCount(); i++) {
		const glm::ivec2 position = {i % size.x, i / size.x};

		const Tilemap::tile_t tile = mainLayerMap(position);

		if (!isItemTile(tile))
			continue;

		const uint16_t material = atlas.findEntryForTile(tile).firstFrame();

		tiles.emplace_back(position, material);
	}

	{
		static const uint16_t neighborsToOffset[] = {0, 4, 2, 9, 1, 8, 5, 11, 3, 7, 6, 13, 10, 14, 12, 15};

		for (int i = 0; i < mainLayerMap.tileCount(); i++) {
			const glm::ivec2 position = {i % size.x, i / size.x};

			const Tilemap::tile_t tile = mainLayerMap(position);

			if (!isBlockTile(tile))
				continue;

			const bool hasLeftNeighbor = isBlockTile(mainLayerMap.getOccupied(position, {-1, 0}, Tile::Concrete));
			const bool hasRightNeighbor = isBlockTile(mainLayerMap.getOccupied(position, {1, 0}, Tile::Concrete));
			const bool hasTopNeighbor = isBlockTile(mainLayerMap.get(position, {0, -1}, Tile::Air));
			const bool hasBottomNeighbor = isBlockTile(mainLayerMap.getOccupied(position, {0, 1}, Tile::Concrete));

			const uint16_t neighbors =
					(hasBottomNeighbor << 3) | (hasTopNeighbor << 2) | (hasRightNeighbor << 1) | hasLeftNeighbor;

			const uint16_t baseMaterial = atlas.findEntryForTile(tile).firstFrame();

			tiles.emplace_back(position, baseMaterial + neighborsToOffset[neighbors]);
		}
	}

	{
		const int &iceMaterial = atlas.findEntryForTile(501).firstFrame();

		for (int i = 0; i < mainLayerMap.tileCount(); i++) {
			const glm::ivec2 position = {i % size.x, i / size.x};

			if (mainLayerMap(position) != Tile::Ice)
				continue;

			tiles.emplace_back(position, iceMaterial);
		}
	}

	for (int i = 0; i < mainLayerMap.tileCount(); i++) {
		const glm::ivec2 position = {i % size.x, i / size.x};

		const Tilemap::tile_t tile = mainLayerMap(position);

		if (!isAnyTeleportTile(tile))
			continue;

		const uint16_t material = atlas.findEntryForTile(tile).firstFrame();

		tiles.emplace_back(position, material);
	}

	for (int i = 0; i < mainLayerMap.tileCount(); i++) {
		const glm::ivec2 position = {i % size.x, i / size.x};

		const Tilemap::tile_t tile = mainLayerMap(position);

		if (!isAnyMonsterTile(tile))
			continue;

		const uint16_t material = atlas.findEntryForTile(tile).firstFrame();

		tiles.emplace_back(position, material);
	}

	for (int i = 0; i < mainLayerMap.tileCount(); i++) {
		const glm::ivec2 position = {i % size.x, i / size.x};

		const Tilemap::tile_t tile = mainLayerMap(position);

		if (tile != Tile::Hero)
			continue;

		const uint16_t material = atlas.findEntryForTile(tile).firstFrame();

		tiles.emplace_back(position, material);
	}

	GLint previousFbo;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousFbo);

	glViewport(0, 0, 1600, 1200);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_renderer->drawTiles(tiles, 1.f);

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