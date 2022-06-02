#include "BaseEditor.hpp"

#include <stdexcept>

GameManager &BaseEditor::gameManager() {
	return _gameManager;
}

WorldFormatManager &BaseEditor::worldFormatManager() {
	return _worldFormatManager;
}

std::shared_ptr<World> BaseEditor::openWorld(const std::filesystem::path &path) {
	std::shared_ptr<IWorldImporter> importer = _worldFormatManager.findImporterForFile(path);

	if (!importer)
		throw std::runtime_error("Can't find suitable importer for '" + path.string() + '\'');

	return importer->load(path, _gameManager);
}

void BaseEditor::saveWorldAs(std::shared_ptr<const World> world, const std::filesystem::path &path) const {

}