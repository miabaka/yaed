#pragma once

#include <memory>

#include "Level.hpp"
#include "World.hpp"

class WorldUtil {
public:
	static std::shared_ptr<Level>
	createLevel(std::shared_ptr<World> world, std::shared_ptr<LevelSkin> skin = {}, const std::string &name = {});
};