#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Level.hpp"

class World {
public:
	explicit World(std::string name = "Unnamed");

	const std::string &name() const;

	const std::vector<std::shared_ptr<Level>> &levels() const;

	void addLevel(const std::shared_ptr<Level> &level);

private:
	std::string _name;
	std::vector<std::shared_ptr<Level>> _levels;
};