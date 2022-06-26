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

	T start() const {
		return _start;
	}

	T stop() const {
		return _stop;
	}

	auto length() const {
		return _stop - _start;
	}
	
	auto inclusiveLength() const {
		return _stop - _start + 1;
	}

	bool inclusiveContains(T value) const {
		return value >= _start && value <= _stop;
	}

private:
	T _start;
	T _stop;
};