#include "TextureSlice.hpp"

TextureSlice::TextureSlice(void *texture, glm::vec2 p1, glm::vec2 p2)
		: _texture(texture),
		  _p1(p1),
		  _p2(p2) {}

void *TextureSlice::texture() const {
	return _texture;
}

glm::vec2 TextureSlice::p1() const {
	return _p1;
}

glm::vec2 TextureSlice::p2() const {
	return _p2;
}