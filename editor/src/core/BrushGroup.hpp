#pragma once

#include <memory>
#include <string>

#include "Brush.hpp"

class BrushGroup {
public:
	explicit BrushGroup(std::string name);

	const std::string &name() const;

	void addBrush(std::shared_ptr<Brush> brush);

private:
	std::string _name;
};