#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "../BaseWorldExporter.hpp"
#include "../IWorldImporter.hpp"
#include "../World.hpp"
#include "WorldFormat.hpp"
#include "WorldFormatInfo.hpp"

class WorldFormatManager {
public:
	WorldFormat &registerWorldFormat(const std::string &id, const WorldFormatInfo &formatInfo);

	std::shared_ptr<IWorldImporter> findImporterForFile(const std::filesystem::path &path) const;

	std::set<std::shared_ptr<BaseWorldExporter>> findExportersForWorld(std::shared_ptr<const World> world) const;

	std::shared_ptr<BaseWorldExporter> findExporterByImporter(std::shared_ptr<const IWorldImporter>) const;

private:
	std::unordered_map<std::string, std::shared_ptr<WorldFormat>> _worldFormats;
};