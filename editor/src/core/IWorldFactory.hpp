#pragma once

#include "game/GameManager.hpp"
#include "Level.hpp"
#include "World.hpp"

class IWorldFactory {
public:
	virtual ~IWorldFactory() = default;

	virtual const std::string &id() const = 0;

	virtual const std::string &name() const = 0;

	virtual std::shared_ptr<World> createWorld(const GameManager &gameManager, std::string name) = 0;

	virtual std::shared_ptr<Level> createLevel(
			const GameManager &gameManager, std::string name, int skinIndex) = 0;
};