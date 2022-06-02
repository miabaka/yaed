#include "BaseEditor.hpp"

#include <stdexcept>

WorldFormatManager &BaseEditor::worldFormatManager() {
	return _worldFormatManager;
}

void BaseEditor::registerGame(std::shared_ptr<IGame> game) {
	_gameManager.registerGame(std::move(game));
}

std::shared_ptr<World> BaseEditor::openWorld(const std::filesystem::path &path) {
	std::shared_ptr<IWorldImporter> importer = _worldFormatManager.findImporterForFile(path);

	if (!importer)
		throw std::runtime_error("Can't find suitable importer for '" + path.string() + '\'');

	return importer->load(path, _gameManager);
}

void BaseEditor::saveWorldAs(std::shared_ptr<const World> world, const std::filesystem::path &path) const {

}