#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CustomDataContainer.hpp"
#include "Layer.hpp"
#include "LevelSkin.hpp"

class World;

class Level : public CustomDataContainer {
	friend class World;

public:
	Level(std::string name, std::shared_ptr<LevelSkin> skin, std::vector<std::shared_ptr<Layer>> layers);

	std::string &name();

	const std::string &name() const;

	std::shared_ptr<LevelSkin> &skin();

	void setSkin(std::shared_ptr<LevelSkin> skin);

	const std::vector<std::shared_ptr<Layer>> &layers() const;

	std::shared_ptr<World> world();

	void setWorld(std::shared_ptr<World> world);

private:
	std::string _name;
	std::shared_ptr<LevelSkin> _skin;
	std::vector<std::shared_ptr<Layer>> _layers;
	std::weak_ptr<World> _world;
};
