#pragma once

#include <filesystem>
#include <memory>
#include <set>
#include <unordered_map>

#include "format/WorldFormatManager.hpp"
#include "game/GameManager.hpp"
#include "game/IGame.hpp"
#include "IWorldImporter.hpp"
#include "BaseWorldExporter.hpp"
#include "World.hpp"
#include "WorldFactoryManager.hpp"

class BaseEditor {
public:
	virtual ~BaseEditor() = default;

	GameManager &gameManager();

	WorldFormatManager &worldFormatManager();

	WorldFactoryManager &worldFactoryManager();

	std::shared_ptr<World> createWorld(const std::string &factoryId, const std::string &name = "Unnamed");

	std::shared_ptr<World> openWorld(const std::filesystem::path &path);

	void saveWorld(std::shared_ptr<const World> world) const;

	void saveWorldAs(
			std::shared_ptr<World> world, const std::filesystem::path &path,
			std::shared_ptr<BaseWorldExporter> exporter = {}) const;

	void closeWorld(std::shared_ptr<World> world);

private:
	GameManager _gameManager;
	WorldFormatManager _worldFormatManager;
	WorldFactoryManager _worldFactoryManager;
	std::set<std::shared_ptr<World>> _worlds;
};