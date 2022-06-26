#pragma once

#include <memory>
#include <set>
#include <unordered_set>

#include "../export/BaseWorldExporter.hpp"
#include "../IWorldImporter.hpp"
#include "WorldFormatInfo.hpp"

class WorldFormat {
public:
	explicit WorldFormat(WorldFormatInfo info);

	const WorldFormatInfo &info() const;

	std::shared_ptr<IWorldImporter> importer() const;

	std::shared_ptr<IWorldExporter> exporter() const;

	void setImporter(std::shared_ptr<IWorldImporter> importer);

	void setExporter(std::shared_ptr<IWorldExporter> exporter);

private:
	WorldFormatInfo _info;
	std::shared_ptr<IWorldImporter> _importer;
	std::shared_ptr<IWorldExporter> _exporter;
};