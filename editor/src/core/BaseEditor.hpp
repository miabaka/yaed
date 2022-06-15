#pragma once

#include <filesystem>
#include <memory>
#include <vector>
#include <unordered_map>

#include "export/IWorldExporter.hpp"
#include "format/WorldFormatManager.hpp"
#include "palette/PaletteIconProviderManager.hpp"
#include "palette/BrushSelectionManager.hpp"
#include "render/TilemapRendererManager.hpp"
#include "game/GameManager.hpp"
#include "game/BaseGame.hpp"
#include "IWorldImporter.hpp"
#include "World.hpp"
#include "WorldFactoryManager.hpp"
#include "Level.hpp"

class BaseEditor {
public:
	BaseEditor();

	virtual ~BaseEditor() = default;

	GameManager &games();

	WorldFormatManager &worldFormats();

	WorldFactoryManager &worldFactories();

	PaletteIconProviderManager &paletteIconProviders();

	TilemapRendererManager &tilemapRenderers();

	std::shared_ptr<BrushSelectionManager> brushSelection();

	const std::vector<std::shared_ptr<World>> &worlds() const;

	std::shared_ptr<World> createWorld(const std::string &factoryId, const std::string &name = "Unnamed");

	std::shared_ptr<World> openWorld(const std::filesystem::path &path);

	void saveWorld(std::shared_ptr<World> world);

	void saveWorldAs(
			std::shared_ptr<World> world, const std::filesystem::path &path,
			std::shared_ptr<IWorldExporter> exporter = {});

	void closeWorld(std::shared_ptr<World> world);

	void saveSelectedWorld();

	void saveSelectedWorldAs(const std::filesystem::path &path, std::shared_ptr<IWorldExporter> exporter = {});

	void closeSelectedWorld();

	void selectLastWorld();

	std::shared_ptr<World> selectedWorld() const;

	std::shared_ptr<Level> selectedLevel() const;

	bool hasSelectedWorld() const;

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

	virtual void onLevelSelectionChange(std::shared_ptr<Level> level);

	virtual void onWorldOpen(std::shared_ptr<World> world, const std::filesystem::path &path);

	/**
	 * Fires on both save and save as
	 */
	virtual void onWorldSave(std::shared_ptr<World> world, const std::filesystem::path &path);

	/**
	 * Fires on any operations with world files (opening, saving)
	 */
	virtual void onWorldFileOperation(std::shared_ptr<World> world, const std::filesystem::path &path);

private:
	GameManager _gameManager;
	WorldFormatManager _worldFormatManager;
	WorldFactoryManager _worldFactoryManager;
	PaletteIconProviderManager _iconProviderManager;
	TilemapRendererManager _tilemapRendererManager;
	std::shared_ptr<BrushSelectionManager> _brushSelection;
	std::vector<std::shared_ptr<World>> _worlds;
	std::weak_ptr<World> _selectedWorld;
	std::weak_ptr<Level> _selectedLevel;
};