#pragma once

#include <memory>
#include <string>
#include <vector>

#include "game/IGame.hpp"
#include "util/CustomDataContainer.hpp"
#include "Level.hpp"

class World : public CustomDataContainer {
public:
	explicit World(std::shared_ptr<IGame> game, std::string name);

	std::string &name();

	const std::string &name() const;

	const std::vector<std::shared_ptr<Level>> &levels() const;

	void addLevel(const std::shared_ptr<Level> &level);

	std::shared_ptr<IGame> game();

	std::shared_ptr<const IGame> game() const;

private:
	std::string _name;
	std::vector<std::shared_ptr<Level>> _levels;
	std::shared_ptr<IGame> _game;
};