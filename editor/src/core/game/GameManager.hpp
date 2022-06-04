#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "BaseGame.hpp"

class GameManager {
public:
	void registerGame(std::shared_ptr<BaseGame> game);

	std::shared_ptr<BaseGame> findGameById(const std::string &id) const;

private:
	std::unordered_map<std::string, std::shared_ptr<BaseGame>> _games;
};