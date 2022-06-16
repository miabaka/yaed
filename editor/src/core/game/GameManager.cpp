#include "GameManager.hpp"

#include <stdexcept>
#include "UnknownGame.hpp"

void GameManager::registerGame(std::shared_ptr<IGame> game) {
	bool inserted = _games.insert({game->id(), game}).second;

	if (!inserted)
		throw std::runtime_error("Game with id '" + game->id() + "' is already registered");
}

std::shared_ptr<IGame> GameManager::findGameById(const std::string &id) const {
	auto it = _games.find(id);

	if (it == _games.end())
		return std::make_shared<UnknownGame>(id);

	return it->second;
}

const std::unordered_map<std::string, std::shared_ptr<IGame>> &GameManager::games() const {
	return _games;
}