#include "SthTilemapRendererInternal.hpp"

#include <fmt/core.h>
#include <stb/image.h>

namespace fs = std::filesystem;

SthTilemapRendererInternal::SthTilemapRendererInternal() {
	setupVbo();
	setupVao();
	setupProgram();
	loadAtlas();
}

SthTilemapRendererInternal::~SthTilemapRendererInternal() {
	glDeleteProgram(_program);
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteTextures(1, &_texAtlas);
	glDeleteTextures(1, &_texFrameOffsets);
}

const Atlas &SthTilemapRendererInternal::getAtlas() {
	return _atlas;
}

void SthTilemapRendererInternal::drawTiles(const std::vector<TileInstance> &tiles, float opacity) const {
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizei>(sizeof(TileInstance) * tiles.size()), tiles.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(_vao);
	glUseProgram(_program);

	// TODO: cache uniform locations
	glUniform1f(glGetUniformLocation(_program, "uOpacity"), opacity);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texAtlas);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, _texFrameOffsets);

	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, static_cast<GLsizei>(tiles.size()));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void SthTilemapRendererInternal::setupVbo() {
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(TileInstance) * 4800, nullptr, GL_DYNAMIC_DRAW);

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
	static const char VERTEX_SOURCE[] = "#version 330 core\n\nlayout (location = 0) in uvec3 aTile;\n\nuniform mat4 uProjection = mat4(.05,0,0,0,0,-1./15,0,0,0,0,-1,0,-1,1,0,1);\n\nflat out int vMaterial;\nout vec2 vTexCoords;\n\nvoid main() {\n    vec2 vertexPosition = vec2(ivec2(gl_VertexID, (gl_VertexID + 1) & 2) / 2);\n\n    bool altScale = bool(aTile.z >> 15);\n\n    vTexCoords = vertexPosition / (altScale ? vec2(20.f, 32.f) : vec2(32.f, 51.2f));\n    vMaterial = int(aTile.z & 0x7fffu);\n\n    vertexPosition = (vertexPosition - 0.5f) * (altScale ? 1.6f : 1.f) + 0.5f;\n    vertexPosition += vec2(aTile.xy);\n\n    gl_Position = uProjection * vec4(vertexPosition, 0.f, 1.f);\n}";
	static const char FRAGMENT_SOURCE[] = "#version 330 core\n\nuniform float uOpacity;\n\nuniform sampler2D texAtlas;\nuniform sampler1D texFrameOffsets;\n\nflat in int vMaterial;\nin vec2 vTexCoords;\n\nout vec4 outColor;\n\nvoid main() {\n    vec2 uvOffset = texelFetch(texFrameOffsets, vMaterial, 0).xy;\n    vec2 uv = uvOffset + vTexCoords;\n    outColor = texture(texAtlas, uv) * vec4(1.f, 1.f, 1.f, uOpacity);\n}"; // wtfff

	_program = glCreateProgram();

	attachShader(_program, GL_VERTEX_SHADER, VERTEX_SOURCE);
	attachShader(_program, GL_FRAGMENT_SHADER, FRAGMENT_SOURCE);

	glLinkProgram(_program);

	{
		std::string log;

		GLint logLength;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLength);

		log.resize(logLength);

		glGetProgramInfoLog(_program, logLength, nullptr, log.data());

		if (!log.empty())
			fmt::print("Shader program log: {}\n", log);
	}

	glUseProgram(_program);

	glUniform1i(glGetUniformLocation(_program, "texAtlas"), 0);
	glUniform1i(glGetUniformLocation(_program, "texFrameOffsets"), 1);

	glUseProgram(0);
}

void SthTilemapRendererInternal::loadAtlas() {
	glm::ivec2 size;
	int channelCount;

	void *pixelData = stbi_load("data/sth_renderer/textures/0.png", &size.x, &size.y, &channelCount, STBI_rgb_alpha);

	if (!pixelData)
		return;

	GLuint texAtlas;
	glGenTextures(1, &texAtlas);

	glBindTexture(GL_TEXTURE_2D, texAtlas);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
	stbi_image_free(pixelData);

	GLuint texFrameOffsets;
	glGenTextures(1, &texFrameOffsets);

	glBindTexture(GL_TEXTURE_1D, texFrameOffsets);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	std::vector<glm::vec2> frameOffsets = _atlas.loadFromFile("data/sth_renderer/atlas.json");

	glTexImage1D(
			GL_TEXTURE_1D, 0, GL_RG32F, static_cast<GLsizei>(frameOffsets.size()), 0, GL_RG, GL_FLOAT,
			frameOffsets.data()
	);

	glBindTexture(GL_TEXTURE_2D, 0);

	_texAtlas = texAtlas;
	_texFrameOffsets = texFrameOffsets;
}