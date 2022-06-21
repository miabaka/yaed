#pragma once

#include <glm/vec2.hpp>

template<typename Tp>
class Rect {
public:
	Rect() = default;

	Rect(Tp p0, Tp p1)
			: _p0(p0), _p1(p1), _hasValue(true) {}

	Tp p0() const {
		return _p0;
	}

	Tp p1() const {
		return _p1;
	}

	bool containsPoint(Tp p) const {
		return p.x >= _p0.x && p.x < _p1.x && p.y >= _p0.y && p.y < _p1.y;
	}

	bool empty() const {
		return !_hasValue;
	}

private:
	Tp _p0{};
	Tp _p1{};
	bool _hasValue = false;
};

using IntRect = Rect<glm::ivec2>;