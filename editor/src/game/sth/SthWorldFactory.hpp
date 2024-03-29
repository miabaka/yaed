#pragma once

#include "../../core/IWorldFactory.hpp"

class SthWorldFactory : public IWorldFactory {
	const std::string &id() const override;

	const std::string &name() const override;

	std::shared_ptr<World> createWorld(std::shared_ptr<IGame> game, std::string name) override;

	std::shared_ptr<Level> createLevel(const IGame &game, std::string name, std::shared_ptr<LevelSkin> skin) override;
};