#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include "../game/IGame.hpp"
#include "../Level.hpp"

class ITilemapRendererContext {
public:
	virtual glm::uvec2 tileSize() const = 0;

	virtual glm::uvec2 viewportSize() const = 0;

	virtual void *viewportTexture() const = 0;

	virtual bool compatibleWith(const IGame &game) = 0;

	virtual void setLevel(std::weak_ptr<Level> level) = 0;

	virtual void render() = 0;
};