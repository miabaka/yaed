#include "WorldFormat.hpp"

WorldFormat::WorldFormat(WorldFormatInfo info)
		: _info(std::move(info)) {}

const WorldFormatInfo &WorldFormat::info() const {
	return _info;
}

std::shared_ptr<IWorldImporter> WorldFormat::importer() const {
	return _importer;
}

std::shared_ptr<BaseWorldExporter> WorldFormat::exporter() const {
	return _exporter;
}

void WorldFormat::setImporter(std::shared_ptr<IWorldImporter> importer) {
	_importer = std::move(importer);
}

void WorldFormat::setExporter(std::shared_ptr<BaseWorldExporter> exporter) {
	_exporter = std::move(exporter);
}