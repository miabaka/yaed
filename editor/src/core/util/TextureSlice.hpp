#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class TextureSlice {
public:
	TextureSlice(void *texture = {}, glm::vec2 p1 = {}, glm::vec2 p2 = {});

	void *texture() const;

	glm::vec2 p1() const;

	glm::vec2 p2() const;

private:
	void *_texture;
	glm::vec2 _p1;
	glm::vec2 _p2;
};