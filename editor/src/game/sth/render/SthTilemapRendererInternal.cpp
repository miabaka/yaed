#include "SthTilemapRendererInternal.hpp"

#include <glm/gtc/type_ptr.hpp>
#include "../SthInternal.hpp"

SthTilemapRendererInternal::SthTilemapRendererInternal()
		: _commonAtlasPair(1.6f),
		  _knobAtlasPair(1.6) {
	setupVbo();
	setupVao();
	setupProgram();
	setupAtlasPairs();
}

SthTilemapRendererInternal::~SthTilemapRendererInternal() {
	glDeleteProgram(_program);
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
}

const AtlasPair &SthTilemapRendererInternal::getAtlasPairFor(const Level &level, AtlasPair::Tag tag) const {
	switch (tag) {
		case AtlasPair::Tag::Common:
			return _commonAtlasPair;

		case AtlasPair::Tag::LadderAndRope:
			return _ladderAndRopeAtlasPair;

		case AtlasPair::Tag::Knobs:
			return _knobAtlasPair;

		case AtlasPair::Tag::Blocks:
			return _blockAtlasPair;

		default:
			break;
	}

	static AtlasPair stub;

	return stub;
}

void SthTilemapRendererInternal::drawTiles(const AtlasPair &atlasPair, const std::vector<TileInstance> &tiles) const {
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// TODO: create separate buffer for each virtual layer to reduce cpu<->gpu synchronization
	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizei>(sizeof(TileInstance) * tiles.size()), tiles.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(_vao);
	glUseProgram(_program);

	// TODO: cache uniform locations
	glUniform1i(glGetUniformLocation(_program, "uFirstAltFrame"), atlasPair.firstAltFrame());
	glUniform1f(glGetUniformLocation(_program, "uTileScale"), atlasPair.defaultTileScale());
	glUniform2fv(glGetUniformLocation(_program, "uCommonScale"), 1, glm::value_ptr(atlasPair.commonScale()));
	glUniform2fv(glGetUniformLocation(_program, "uAltScale"), 1, glm::value_ptr(atlasPair.altScale()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlasPair.commonTexture());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, atlasPair.altTexture());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, atlasPair.frameOffsetTexture());

	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, static_cast<GLsizei>(tiles.size()));

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void SthTilemapRendererInternal::setupVbo() {
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(TileInstance) * 2400, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SthTilemapRendererInternal::setupVao() {
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribDivisor(0, 1);
	glVertexAttribIPointer(0, 3, GL_UNSIGNED_SHORT, sizeof(TileInstance), (void *) 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

static void attachShader(GLuint program, GLenum type, const char *source) {
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	glAttachShader(program, shader);

	glDeleteShader(shader);
}

void SthTilemapRendererInternal::setupProgram() {
	static const char VERTEX_SOURCE[] = "#version 330 core\n\nlayout (location = 0) in ivec3 a;\n\nuniform float uTileScale;\nuniform mat4 uProjection = mat4(.05,0,0,0,0,-.0666667,0,0,0,0,-1,0,-1,1,0,1);\n\nflat out int vMaterial;\nout vec2 vTexCoords;\n\nvoid main(){int i=gl_VertexID;vec2 d=vec2(ivec2(i,(i+1)&2)/2);vTexCoords=d;vMaterial=a.b;gl_Position=uProjection*vec4((d-.5)*uTileScale+.5+a.rg,0,1);}";
	static const char FRAGMENT_SOURCE[] = "#version 330 core\n\nuniform int uFirstAltFrame = 0;\nuniform vec2 uCommonScale;\nuniform vec2 uAltScale;\n\nuniform sampler2D texCommon;\nuniform sampler2D texAlt;\nuniform sampler1D texFrameOffsets;\n\nflat in int vMaterial;\nin vec2 vTexCoords;\n\nout vec4 outColor;\n\nvoid main() {\n    vec2 uvOffset = texelFetch(texFrameOffsets, vMaterial, 0).xy;\n\n    bool useAlt = vMaterial >= uFirstAltFrame;\n    vec2 uv = uvOffset + vTexCoords * (useAlt ? uAltScale : uCommonScale);\n\n    outColor = texture(useAlt ? texAlt : texCommon, uv);\n}"; // втффф

	_program = glCreateProgram();

	attachShader(_program, GL_VERTEX_SHADER, VERTEX_SOURCE);
	attachShader(_program, GL_FRAGMENT_SHADER, FRAGMENT_SOURCE);

	glLinkProgram(_program);

	glUseProgram(_program);

	glUniform1i(glGetUniformLocation(_program, "texCommon"), 0);
	glUniform1i(glGetUniformLocation(_program, "texAlt"), 1);
	glUniform1i(glGetUniformLocation(_program, "texFrameOffsets"), 2);

	glUseProgram(0);
}

void SthTilemapRendererInternal::setupAtlasPairs() {
	using namespace SthInternal;

	{
		const std::vector<InputAtlasEntry> commonEntries = {
				{{Tile::Bonus},        {0,    0},   {64, 64}, 32, 4},
				{{Tile::Hero},         {256,  0},   {64, 64}, 32, 4},
				{{Tile::BombItem},     {512,  0},   {64, 64}, 32, 4},
				{{Tile::FakeHeroItem}, {768,  0},   {64, 64}, 32, 4},
				{{Tile::KeyItem},      {1024, 0},   {64, 64}, 32, 4},
				{{Tile::TrapItem},     {1280, 0},   {64, 64}, 32, 4},
				{{512},                {0,    512}, {64, 64}, 32, 4},
				{{513},                {256,  512}, {64, 64}, 32, 4},
				{{514},                {512,  512}, {64, 64}, 32, 4},
				{{515},                {768,  512}, {64, 64}, 32, 4},
				{{516},                {1024, 512}, {64, 64}, 32, 4},
				{{517},                {1280, 512}, {64, 64}, 32, 4}
		};

		std::unordered_set<Tilemap::tile_t> teleportIns;
		std::unordered_set<Tilemap::tile_t> teleportOuts;

		for (Tilemap::tile_t tile = Tile::TeleportIn0; tile <= Tile::TeleportInE; tile++)
			teleportIns.insert(tile);

		for (Tilemap::tile_t tile = Tile::TeleportOut0; tile <= Tile::TeleportOutE; tile++)
			teleportOuts.insert(tile);

		const std::vector<InputAtlasEntry> altEntries = {
				{{Tile::LockedExit},      {0,    0},   {64, 64}, 32, 4},
				{{Tile::HiddenExit},      {256,  0},   {64, 64}, 32, 4},
				{{Tile::Gem0},            {512,  0},   {64, 64}, 32, 4},
				{{Tile::Gem1},            {768,  0},   {64, 64}, 32, 4},
				{{Tile::Gem2},            {1024, 0},   {64, 64}, 32, 4},
				{{Tile::Gem3},            {0,    512}, {64, 64}, 32, 4},
				{{Tile::Gem4},            {256,  512}, {64, 64}, 32, 4},
				{{Tile::Gem5},            {512,  512}, {64, 64}, 32, 4},
				{{518},                   {768,  512}, {64, 64}, 32, 4},
				{std::move(teleportIns),  {1024, 512}, {64, 64}, 16, 4},
				{std::move(teleportOuts), {1024, 768}, {64, 64}, 16, 4}
		};

		_commonAtlasPair.appendEntries(commonEntries, {1536, 1024});

		_commonAtlasPair.beginAltOffsets();
		_commonAtlasPair.appendEntries(altEntries, {1280, 1024});

		_commonAtlasPair.submitFrameOffsets();
	}

	_commonAtlasPair.loadTexture("data/sth_renderer/textures/common.png", AtlasPair::TextureSlot::Common);
	_commonAtlasPair.loadTexture("data/sth_renderer/textures/0_0.png", AtlasPair::TextureSlot::Alt);

	{
		const std::vector<InputAtlasEntry> altEntries = {
				{{Tile::Rope},                       {0,  0}, {40, 40}, 1, 1},
				{{Tile::Ladder, Tile::HiddenLadder}, {40, 0}, {40, 40}, 1, 1}
		};

		_ladderAndRopeAtlasPair.beginAltOffsets();
		_ladderAndRopeAtlasPair.appendEntries(altEntries, {80, 40});

		_ladderAndRopeAtlasPair.submitFrameOffsets();
	}

	_ladderAndRopeAtlasPair.loadTexture("data/sth_renderer/textures/0_1.png", AtlasPair::TextureSlot::Alt);

	{
		const std::vector<InputAtlasEntry> altEntries = {
				{{512}, {0, 0},  {64, 64}, 3, 3},
				{{513}, {0, 64}, {64, 64}, 3, 3}
		};

		_knobAtlasPair.beginAltOffsets();
		_knobAtlasPair.appendEntries(altEntries, {192, 128});

		_knobAtlasPair.submitFrameOffsets();
	}

	_knobAtlasPair.loadTexture("data/sth_renderer/textures/0_2.png", AtlasPair::TextureSlot::Alt);


	{
		const std::vector<InputAtlasEntry> altEntries = {
				{{Tile::Ground, Tile::Hole, Tile::Ice}, {0, 0},  {40, 40}, 16, 16},
				{{Tile::AltGround},                     {0, 40}, {40, 40}, 16, 16},
				{{Tile::Concrete},                      {0, 80}, {40, 40}, 16, 16}
		};

		_blockAtlasPair.beginAltOffsets();
		_blockAtlasPair.appendEntries(altEntries, {640, 120});

		_blockAtlasPair.submitFrameOffsets();
	}

	_blockAtlasPair.loadTexture("data/sth_renderer/textures/0_3.png", AtlasPair::TextureSlot::Alt);
}