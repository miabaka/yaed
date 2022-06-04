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
	WorldFormat &registerFormat(const std::string &id, const WorldFormatInfo &formatInfo);

	std::shared_ptr<IWorldImporter> findImporterForFile(const std::filesystem::path &path) const;

	std::set<std::shared_ptr<BaseWorldExporter>> findExportersForWorld(std::shared_ptr<const World> world) const;

	std::shared_ptr<BaseWorldExporter> findAssociatedExporter(std::shared_ptr<const IWorldImporter> importer) const;

	const std::unordered_map<std::string, std::shared_ptr<WorldFormat>> &formats() const;

private:
	std::unordered_map<std::string, std::shared_ptr<WorldFormat>> _worldFormats;
};