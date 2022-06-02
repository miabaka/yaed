#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Layer.hpp"

class Level {
public:
	Level(std::string name, int skinIndex, std::vector<std::shared_ptr<Layer>> layers);

	int skinIndex() const;

	const std::string &name() const;

	const std::vector<std::shared_ptr<Layer>> &layers() const;

private:
	std::string _name;
	int _skinIndex;
	std::vector<std::shared_ptr<Layer>> _layers;
};
