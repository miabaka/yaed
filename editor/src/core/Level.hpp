#pragma once

#include <memory>
#include <string>
#include <vector>

#include "util/CustomDataContainer.hpp"
#include "Layer.hpp"
#include "LevelSkin.hpp"

class World;

class Level : public CustomDataContainer {
	friend class World;

	friend class WorldUtil;

public:
	Level(std::string name, std::shared_ptr<LevelSkin> skin, std::vector<std::shared_ptr<Layer>> layers);

	std::string &name();

	const std::string &name() const;

	std::shared_ptr<LevelSkin> skin() const;

	void setSkin(std::shared_ptr<LevelSkin> skin);

	std::shared_ptr<Layer> selectedLayer() const;

	void selectLayer(std::shared_ptr<Layer> layer);

	const std::vector<std::shared_ptr<Layer>> &layers() const;

	std::shared_ptr<World> world();

	bool markedForRemoval() const;

	void markForRemoval(bool marked = true);

private:
	std::string _name;
	std::shared_ptr<LevelSkin> _skin;
	std::vector<std::shared_ptr<Layer>> _layers;
	std::weak_ptr<World> _world;
	std::weak_ptr<Layer> _selectedLayer;
	bool _markedForRemoval = false;

	void setWorld(std::shared_ptr<World> world);
};
