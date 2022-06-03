#pragma once

#include <stdexcept>

template<typename T>
class Range {
public:
	Range(T value)
			: _start(value),
			  _stop(value) {}

	Range(T start, T stop)
			: _start(start),
			  _stop(stop) {
		if (stop < start)
			throw std::invalid_argument("Range stop must be greater or equal to range start");
	}

	T start() {
		return _start;
	}

	T stop() {
		return _stop;
	}

private:
	T _start;
	T _stop;
};