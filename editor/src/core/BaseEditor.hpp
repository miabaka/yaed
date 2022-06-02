#pragma once

#include <filesystem>
#include <memory>
#include <set>
#include <unordered_map>

#include "game/GameManager.hpp"
#include "game/IGame.hpp"
#include "IWorldImporter.hpp"
#include "BaseWorldExporter.hpp"
#include "World.hpp"
#include "format/WorldFormatManager.hpp"

class BaseEditor {
public:
	virtual ~BaseEditor() = default;

	GameManager &gameManager();

	WorldFormatManager &worldFormatManager();

	std::shared_ptr<World> openWorld(const std::filesystem::path &path);

	void saveWorld(std::shared_ptr<const World> world) const;

	void saveWorldAs(
			std::shared_ptr<World> world, const std::filesystem::path &path,
			std::shared_ptr<BaseWorldExporter> exporter = {}) const;

	void closeWorld(std::shared_ptr<World> world);

private:
	GameManager _gameManager;
	WorldFormatManager _worldFormatManager;
	std::set<std::shared_ptr<World>> _worlds;
};