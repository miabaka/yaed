#pragma once

#include <string>
#include "CustomDataContainer.hpp"

class LevelSkin : public CustomDataContainer {
public:
	explicit LevelSkin(int id);

	LevelSkin(int id, std::string name);

	int id() const;

	const std::string &name() const;

private:
	int _id;
	std::string _name;
};