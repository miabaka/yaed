#pragma once

#include <filesystem>
#include <memory>
#include <set>
#include <unordered_map>

#include "games/GameManager.hpp"
#include "games/IGame.hpp"
#include "IWorldImporter.hpp"
#include "BaseWorldExporter.hpp"
#include "World.hpp"
#include "formats/WorldFormatManager.hpp"

class BaseEditor {
public:
	virtual ~BaseEditor() = default;

	WorldFormatManager &worldFormatManager();

	void registerGame(std::shared_ptr<IGame> game);

	std::shared_ptr<World> openWorld(const std::filesystem::path &path);

	void saveWorldAs(std::shared_ptr<const World> world, const std::filesystem::path &path) const;

private:
	GameManager _gameManager;
	WorldFormatManager _worldFormatManager;
};