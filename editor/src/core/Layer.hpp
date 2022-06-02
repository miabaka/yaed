#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include "LayerTemplate.hpp"
#include "Tilemap.hpp"

class Layer {
public:
	Layer(std::shared_ptr<const LayerTemplate> aTemplate, glm::ivec2 size);

	Tilemap &tilemap();

	const Tilemap &tilemap() const;

	const std::string &name() const;

private:
	std::shared_ptr<const LayerTemplate> _template;
	Tilemap _tilemap;
};