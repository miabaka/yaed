#pragma once

#include <memory>
#include <unordered_set>

#include "IWorldExporter.hpp"
#include "IWorldImporter.hpp"
#include "WorldFormatInfo.hpp"

class WorldFormat {
public:
	explicit WorldFormat(WorldFormatInfo info);

	const WorldFormatInfo &info() const;

	const std::unordered_set<std::shared_ptr<IWorldImporter>> &importers() const;

	void registerImporter(std::shared_ptr<IWorldImporter> importer);

	void registerExporter(std::shared_ptr<IWorldExporter> exporter) {}

private:
	WorldFormatInfo _info;
	std::unordered_set<std::shared_ptr<IWorldImporter>> _importers;
};