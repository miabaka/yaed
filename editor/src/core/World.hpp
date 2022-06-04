#pragma once

#include <memory>
#include <string>
#include <vector>

#include "game/IGame.hpp"
#include "Level.hpp"
#include "ICustomData.hpp"

class World {
public:
	explicit World(std::shared_ptr<IGame> game, std::string name);

	const std::string &name() const;

	const std::vector<std::shared_ptr<Level>> &levels() const;

	void addLevel(const std::shared_ptr<Level> &level);

	std::shared_ptr<const IGame> game() const;

	template<typename T>
	T &customData() {
		return *dynamic_cast<T *>(_customData.get());
	}

	template<typename T>
	const T &customData() const {
		return *dynamic_cast<T *>(_customData.get());
	}

	void setCustomData(std::unique_ptr<ICustomData> customData);

private:
	std::string _name;
	std::vector<std::shared_ptr<Level>> _levels;
	std::shared_ptr<IGame> _game;
	std::unique_ptr<ICustomData> _customData;
};