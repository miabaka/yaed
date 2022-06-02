#include "WorldFormatManager.hpp"

WorldFormat &WorldFormatManager::registerWorldFormat(const std::string &id, const WorldFormatInfo &formatInfo) {
	auto format = std::make_shared<WorldFormat>(formatInfo);

	bool inserted = _worldFormats.insert({id, format}).second;

	if (!inserted)
		throw std::runtime_error("World format with id '" + id + "' is already registered");

	return *format;
}

std::shared_ptr<IWorldImporter> WorldFormatManager::findImporterForFile(const std::filesystem::path &path) const {
	for (auto &[_, format]: _worldFormats) {
		std::shared_ptr<IWorldImporter> importer = format->importer();

		if (!importer)
			continue;

		if (importer->probe(path))
			return importer;
	}

	return {};
}

std::set<std::shared_ptr<BaseWorldExporter>>
WorldFormatManager::findExportersForWorld(std::shared_ptr<const World> world) const {
	const std::string &gameId = world->game()->id();

	std::set<std::shared_ptr<BaseWorldExporter>> result;

	for (auto &[_, format]: _worldFormats) {
		std::shared_ptr<BaseWorldExporter> exporter = format->exporter();

		if (!(exporter && exporter->gameIsSupported(gameId)))
			continue;

		result.insert(exporter);
	}

	return result;
}

std::shared_ptr<BaseWorldExporter>
WorldFormatManager::findExporterByImporter(std::shared_ptr<const IWorldImporter>) const {
	return {};
}