#pragma once

#include <memory>

#include "game/GameManager.hpp"
#include "Level.hpp"
#include "LevelSkin.hpp"
#include "World.hpp"

// TODO: add property with supported game ids
class IWorldFactory {
	friend class World;

public:
	virtual ~IWorldFactory() = default;

	virtual const std::string &id() const = 0;

	virtual const std::string &name() const = 0;

	virtual std::shared_ptr<World> createWorld(std::shared_ptr<IGame> game, std::string name) = 0;

private:
	virtual std::shared_ptr<Level>
	createLevel(const IGame &game, std::string name, std::shared_ptr<LevelSkin> skin) = 0;
};