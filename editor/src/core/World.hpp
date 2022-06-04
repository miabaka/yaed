#pragma once

#include <memory>
#include <string>
#include <vector>

#include "game/BaseGame.hpp"
#include "CustomDataContainer.hpp"
#include "Level.hpp"

class World : public CustomDataContainer {
public:
	explicit World(std::shared_ptr<BaseGame> game, std::string name);

	std::string &name();

	const std::string &name() const;

	const std::vector<std::shared_ptr<Level>> &levels() const;

	void addLevel(const std::shared_ptr<Level> &level);

	std::shared_ptr<BaseGame> game();

	std::shared_ptr<const BaseGame> game() const;

private:
	std::string _name;
	std::vector<std::shared_ptr<Level>> _levels;
	std::shared_ptr<BaseGame> _game;
};