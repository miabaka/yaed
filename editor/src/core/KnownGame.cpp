#include "KnownGame.hpp"

bool KnownGame::known() const {
	return true;
}

const std::string &KnownGame::id() const {
	return _id;
}

const std::string &KnownGame::name() const {
	return _name;
}

KnownGame::KnownGame(std::string id, std::string name)
		: _id(std::move(id)),
		  _name(std::move(name)) {}