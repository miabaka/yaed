#include "WorldUtil.hpp"

std::shared_ptr<Level>
WorldUtil::createLevel(std::shared_ptr<World> world, std::shared_ptr<LevelSkin> skin, const std::string &name) {
	std::shared_ptr<Level> level = world->createLevel(std::move(skin), name);

	if (level)
		level->setWorld(world);

	return level;
}