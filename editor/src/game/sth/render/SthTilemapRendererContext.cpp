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

static inline bool isGemTile(Tilemap::tile_t tile) {
	return tile >= Tile::Gem0 && tile <= Tile::Gem5;
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

	const AtlasPair &commonAtlasPair = _renderer->getAtlasPairFor(*level, AtlasPair::Tag::Common);
	const AtlasPair &ladderAndRopeAtlasPair = _renderer->getAtlasPairFor(*level, AtlasPair::Tag::LadderAndRope);
	const AtlasPair &knobAtlasPair = _renderer->getAtlasPairFor(*level, AtlasPair::Tag::Knobs);
	const AtlasPair &blockAtlasPair = _renderer->getAtlasPairFor(*level, AtlasPair::Tag::Blocks);

	// TODO: use preallocated memory
	std::vector<SthTilemapRendererInternal::TileInstance> ladderAndRopeTiles;
	std::vector<SthTilemapRendererInternal::TileInstance> knobTiles;
	std::vector<SthTilemapRendererInternal::TileInstance> gemTiles;
	std::vector<SthTilemapRendererInternal::TileInstance> blockTiles;
	std::vector<SthTilemapRendererInternal::TileInstance> commonTiles;

	ladderAndRopeTiles.reserve(1200);
	knobTiles.reserve(1200);
	gemTiles.reserve(1200);
	blockTiles.reserve(1200);
	commonTiles.reserve(2400);

	for (int i = 0; i < gemLayerMap.tileCount(); i++) {
		glm::ivec2 position = {i % size.x, i / size.x};

		Tilemap::tile_t tile = gemLayerMap(position);

		if (tile < Tile::Gem0 || tile > Tile::Gem5)
			continue;

		int frame = commonAtlasPair.findEntryForTile(tile).firstFrame();

		gemTiles.emplace_back(position, frame);
	}

	for (int i = 0; i < mainLayerMap.tileCount(); i++) {
		glm::ivec2 position = {i % size.x, i / size.x};

		Tilemap::tile_t tile = mainLayerMap(position);

		if (!(isLadderTile(tile) || tile == Tile::Rope))
			continue;

		int frame = ladderAndRopeAtlasPair.findEntryForTile(tile).firstFrame();

		ladderAndRopeTiles.emplace_back(position, frame);
	}

	const int firstRopeKnobFrame = knobAtlasPair.findEntryForTile(512).firstFrame();
	const int firstLadderKnobFrame = knobAtlasPair.findEntryForTile(513).firstFrame();

	for (int i = 0; i < mainLayerMap.tileCount(); i++) {
		glm::ivec2 position = {i % size.x, i / size.x};

		Tilemap::tile_t tile = mainLayerMap(position);

		int frame;

		if (isLadderTile(tile)) {
			Tilemap::tile_t topTile = mainLayerMap(position + glm::ivec2(0, -1));
			Tilemap::tile_t bottomTile = mainLayerMap(position + glm::ivec2(0, 1));

			bool hasTopNeighbor = isLadderTile(topTile);
			bool hasBottomNeighbor = isLadderTile(bottomTile);

			if (hasTopNeighbor && hasBottomNeighbor)
				continue;

			frame = firstLadderKnobFrame + ((hasTopNeighbor << 1) | hasBottomNeighbor);
		} else if (tile == Tile::Rope) {
			Tilemap::tile_t leftTile = mainLayerMap(position + glm::ivec2(-1, 0));
			Tilemap::tile_t rightTile = mainLayerMap(position + glm::ivec2(1, 0));

			bool hasLeftNeighbor = (leftTile == Tile::Rope);
			bool hasRightNeighbor = (rightTile == Tile::Rope);

			if (hasLeftNeighbor && hasRightNeighbor)
				continue;

			frame = firstRopeKnobFrame + ((hasLeftNeighbor << 1) | hasRightNeighbor);
		} else {
			continue;
		}

		knobTiles.emplace_back(position, frame);
	}

	static const int blockDockingMapping[] = {0, 4, 2, 9, 1, 8, 5, 11, 3, 7, 6, 13, 10, 14, 12, 15};

	for (int i = 0; i < gemLayerMap.tileCount(); i++) {
		glm::ivec2 position = {i % size.x, i / size.x};

		Tilemap::tile_t tile = mainLayerMap(position);

		if (!isBlockTile(tile))
			continue;

		bool hasLeftNeighbor = isBlockTile(mainLayerMap(position + glm::ivec2(-1, 0)));
		bool hasRightNeighbor = isBlockTile(mainLayerMap(position + glm::ivec2(1, 0)));
		bool hasTopNeighbor = isBlockTile(mainLayerMap(position + glm::ivec2(0, -1)));
		bool hasBottomNeighbor = isBlockTile(mainLayerMap(position + glm::ivec2(0, 1)));

		int baseFrame = blockAtlasPair.findEntryForTile(tile).firstFrame();
		int variant = (hasBottomNeighbor << 3) | (hasTopNeighbor << 2) | (hasRightNeighbor << 1) | hasLeftNeighbor;

		int frame = baseFrame + blockDockingMapping[variant];

		blockTiles.emplace_back(position, frame);
	}

	{
		const AtlasEntry &gemSignEntry = commonAtlasPair.findEntryForTile(518);

		for (int i = 0; i < mainLayerMap.tileCount(); i++) {
			glm::ivec2 position = {i % size.x, i / size.x};

			Tilemap::tile_t tile = mainLayerMap(position);

			if (isSolidTile(tile))
				continue;

			Tilemap::tile_t bottomMainTile = mainLayerMap(position + glm::ivec2{0, 1});
			Tilemap::tile_t bottomGemTile = gemLayerMap(position + glm::ivec2{0, 1});

			if (!(isBlockTile(bottomMainTile) && isGemTile(bottomGemTile)))
				continue;

			commonTiles.emplace_back(position, gemSignEntry.firstFrame());
		}
	}

	{
		const AtlasEntry &iceEntry = commonAtlasPair.findEntryForTile(517);
		const AtlasEntry &monsterEntry = commonAtlasPair.findEntryForTile(514);
		const AtlasEntry &secondMonsterEntry = commonAtlasPair.findEntryForTile(513);

		for (int i = 0; i < mainLayerMap.tileCount(); i++) {
			glm::ivec2 position = {i % size.x, i / size.x};

			Tilemap::tile_t tile = mainLayerMap(position);

			if (tile < Tile::HiddenExit || (tile > Tile::FakeHeroItem && tile != Tile::Bonus))
				continue;

			const AtlasEntry *entry;

			if (tile == Tile::Ice)
				entry = &iceEntry;
			else if (isMonsterTile(tile))
				entry = &monsterEntry;
			else if (isSecondMonsterTile(tile))
				entry = &secondMonsterEntry;
			else
				entry = &commonAtlasPair.findEntryForTile(tile);

			commonTiles.emplace_back(position, entry->firstFrame());
		}
	}

	GLint previousFbo;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousFbo);

	glViewport(0, 0, 1600, 1200);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const bool gemLayerSelected = (level->selectedLayer() == level->layers()[0]);
	const float commonLayerOpacity = gemLayerSelected ? 0.5f : 1.f;

	_renderer->drawTiles(ladderAndRopeAtlasPair, ladderAndRopeTiles, commonLayerOpacity);
	_renderer->drawTiles(knobAtlasPair, knobTiles, commonLayerOpacity);

	if (!gemLayerSelected)
		_renderer->drawTiles(commonAtlasPair, gemTiles);

	_renderer->drawTiles(blockAtlasPair, blockTiles, commonLayerOpacity);
	_renderer->drawTiles(commonAtlasPair, commonTiles, commonLayerOpacity);

	if (gemLayerSelected)
		_renderer->drawTiles(commonAtlasPair, gemTiles);

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