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
#include "WorldFormat.hpp"
#include "WorldFormatInfo.hpp"

class BaseEditor {
public:
	virtual ~BaseEditor() = default;

	WorldFormat &registerWorldFormat(const std::string &id, const WorldFormatInfo &formatInfo);

	void registerGame(std::shared_ptr<IGame> game);

	std::shared_ptr<IWorldImporter> findImporterForFile(const std::filesystem::path &path) const;

	std::set<std::shared_ptr<BaseWorldExporter>> findExportersForWorld(std::shared_ptr<const World> world) const;

	std::shared_ptr<World> openWorld(const std::filesystem::path &path);

	void saveWorldAs(std::shared_ptr<const World> world, const std::filesystem::path &path) const;

private:
	std::unordered_map<std::string, std::shared_ptr<WorldFormat>> _worldFormats;
	GameManager _gameManager;
};