#pragma once

#include <glm/vec4.hpp>

class TextureSlice {
public:
	TextureSlice(void *texture, glm::vec4 texCoords);

	void *texture() const;

	glm::vec4 texCoords() const;

private:
	void *_texture;
	glm::vec4 _texCoords;
};