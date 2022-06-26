#pragma once

#include <string>

class ITool {
public:
	virtual ~ITool() = default;

	virtual const std::string &name() const = 0;
};