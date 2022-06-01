#include "Editor.hpp"

#include <stdexcept>

WorldFormat &Editor::registerWorldFormat(const std::string &id, const WorldFormatInfo &formatInfo) {
	auto format = std::make_shared<WorldFormat>(formatInfo);

	bool inserted = _worldFormats.insert({id, format}).second;

	if (!inserted)
		throw std::runtime_error("World format with id '" + id + "' is already registered");

	return *format;
}

std::shared_ptr<IWorldImporter> Editor::findImporterForFile(const std::filesystem::path &path) const {
	for (auto &[_, pFormat]: _worldFormats) {
		for (auto &pImporter: pFormat->importers()) {
			if (pImporter->probe(path))
				return pImporter;
		}
	}

	return {};
}

std::shared_ptr<World> Editor::openWorld(const std::filesystem::path &path) {
	std::shared_ptr<IWorldImporter> importer = findImporterForFile(path);

	if (!importer)
		throw std::runtime_error("Can't find suitable importer for '" + path.string() + '\'');

	return importer->import(path);
}