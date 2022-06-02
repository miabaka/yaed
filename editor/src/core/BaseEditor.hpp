#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>

#include "IGame.hpp"
#include "World.hpp"
#include "WorldFormat.hpp"
#include "WorldFormatInfo.hpp"

class BaseEditor {
public:
	virtual ~BaseEditor() = default;

	WorldFormat &registerWorldFormat(const std::string &id, const WorldFormatInfo &formatInfo);

	void registerGame(std::shared_ptr<IGame> game);

	std::shared_ptr<IWorldImporter> findImporterForFile(const std::filesystem::path &path) const;

	std::shared_ptr<World> openWorld(const std::filesystem::path &path);

private:
	std::unordered_map<std::string, std::shared_ptr<WorldFormat>> _worldFormats;
};