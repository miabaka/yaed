#include "WorldFormat.hpp"

WorldFormat::WorldFormat(WorldFormatInfo info)
		: _info(std::move(info)) {}

const WorldFormatInfo &WorldFormat::info() const {
	return _info;
}

const std::unordered_set<std::shared_ptr<IWorldImporter>> &WorldFormat::importers() const {
	return _importers;
}

void WorldFormat::registerImporter(std::shared_ptr<IWorldImporter> importer) {
	_importers.insert(importer);
}