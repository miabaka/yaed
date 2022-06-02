#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "IGame.hpp"

class GameManager {
public:
	void registerGame(std::shared_ptr<IGame> game);

	std::shared_ptr<IGame> findGameById(const std::string &id) const;

private:
	std::unordered_map<std::string, std::shared_ptr<IGame>> _games;
};