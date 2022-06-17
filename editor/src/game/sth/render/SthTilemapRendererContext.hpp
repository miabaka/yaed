#pragma once

#include <GL/gl3w.h>

#include "../../../core/render/ITilemapRendererContext.hpp"
#include "../../../core/Level.hpp"
#include "SthTilemapRendererInternal.hpp"

class SthTilemapRendererContext : public ITilemapRendererContext {
	friend class SthTilemapRenderer;

public:
	~SthTilemapRendererContext();

	glm::ivec2 tileSize() const override;

	glm::ivec2 viewportSize() const override;

	void *viewportTexture() const override;

	bool compatibleWith(const IGame &game) override;

	void setLevel(std::weak_ptr<Level> level) override;

	void render() override;

private:
	GLuint _fbo = 0;
	GLuint _texture = 0;
	std::weak_ptr<Level> _level;
	std::shared_ptr<SthTilemapRendererInternal> _renderer;

	SthTilemapRendererContext(std::shared_ptr<SthTilemapRendererInternal> renderer);

	static GLuint createTexture();

	static GLuint createFrameBuffer(GLuint texture);
};