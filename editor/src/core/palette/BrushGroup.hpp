#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Brush.hpp"

class BrushGroup {
public:
	explicit BrushGroup(std::string name);

	const std::string &name() const;

	const std::vector<std::shared_ptr<Brush>> &brushes() const;

	void addBrush(std::shared_ptr<Brush> brush);

private:
	std::string _name;
	std::vector<std::shared_ptr<Brush>> _brushes;
};