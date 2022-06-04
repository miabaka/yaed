#include "LevelSkin.hpp"

#include <fmt/format.h>

LevelSkin::LevelSkin(int id)
		: _id(id),
		  _name(fmt::format("Unknown ({})", id)) {}

LevelSkin::LevelSkin(int id, std::string name)
		: _id(id),
		  _name(std::move(name)) {}

int LevelSkin::id() const {
	return _id;
}

const std::string &LevelSkin::name() const {
	return _name;
}