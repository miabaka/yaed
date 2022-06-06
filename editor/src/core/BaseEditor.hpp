#pragma once

#include <filesystem>
#include <memory>
#include <vector>
#include <unordered_map>

#include "format/WorldFormatManager.hpp"
#include "game/GameManager.hpp"
#include "game/BaseGame.hpp"
#include "IWorldImporter.hpp"
#include "BaseWorldExporter.hpp"
#include "Level.hpp"
#include "World.hpp"
#include "WorldFactoryManager.hpp"

class BaseEditor {
public:
	virtual ~BaseEditor() = default;

	GameManager &gameManager();

	WorldFormatManager &worldFormatManager();

	WorldFactoryManager &worldFactoryManager();

	const std::vector<std::shared_ptr<World>> &worlds() const;

	std::shared_ptr<World> createWorld(const std::string &factoryId, const std::string &name = "Unnamed");

	std::shared_ptr<World> openWorld(const std::filesystem::path &path);

	void saveWorld(std::shared_ptr<const World> world) const;

	void saveWorldAs(
			std::shared_ptr<World> world, const std::filesystem::path &path,
			std::shared_ptr<BaseWorldExporter> exporter = {}) const;

	void closeWorld(std::shared_ptr<World> world);

	void saveSelectedWorld() const;

	void saveSelectedWorldAs(const std::filesystem::path &path, std::shared_ptr<BaseWorldExporter> exporter = {}) const;

	void closeSelectedWorld();

	std::shared_ptr<World> selectedWorld() const;

	std::shared_ptr<Level> selectedLevel() const;

	/**
	 * Select world and reset the level selection
	 */
	void selectWorld(std::shared_ptr<World> world);

	/**
	 * Select level and its world
	 */
	void selectLevel(std::shared_ptr<Level> level);

protected:
	virtual void onWorldSelectionChange(std::shared_ptr<World> world);

private:
	GameManager _gameManager;
	WorldFormatManager _worldFormatManager;
	WorldFactoryManager _worldFactoryManager;
	std::vector<std::shared_ptr<World>> _worlds;
	std::weak_ptr<World> _selectedWorld;
	std::weak_ptr<Level> _selectedLevel;
};