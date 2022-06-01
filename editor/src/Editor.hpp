#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>

#include "World.hpp"
#include "WorldFormat.hpp"
#include "WorldFormatInfo.hpp"

class Editor {
public:
	WorldFormat &registerWorldFormat(const std::string &id, const WorldFormatInfo &formatInfo);

	std::shared_ptr<IWorldImporter> findImporterForFile(const std::filesystem::path &path) const;

	std::shared_ptr<World> openWorld(const std::filesystem::path &path);

private:
	std::unordered_map<std::string, std::shared_ptr<WorldFormat>> _worldFormats;
};