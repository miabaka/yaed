#include "TextureSlice.hpp"

TextureSlice::TextureSlice(void *texture, glm::vec4 texCoords)
		: _texture(texture),
		  _texCoords(texCoords) {}

void *TextureSlice::texture() const {
	return _texture;
}

glm::vec4 TextureSlice::texCoords() const {
	return _texCoords;
}