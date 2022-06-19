#include "NewWorldDialogState.hpp"

void NewWorldDialogState::reset() {
	_chosenName.clear();
	_selectedGame.reset();
	_selectedFactory.reset();
	_selectedFormat.reset();
}

bool NewWorldDialogState::valid() const {
	return !(_selectedGame.expired() || _selectedFactory.expired());
}

std::string &NewWorldDialogState::chosenName() {
	return _chosenName;
}

const std::string &NewWorldDialogState::chosenName() const {
	return _chosenName;
}

std::shared_ptr<IGame> NewWorldDialogState::selectedGame() const {
	return _selectedGame.lock();
}

std::shared_ptr<IWorldFactory> NewWorldDialogState::selectedFactory() const {
	return _selectedFactory.lock();
}

std::shared_ptr<WorldFormat> NewWorldDialogState::selectedFormat() const {
	return _selectedFormat.lock();
}

const char *NewWorldDialogState::selectedGameRawName() const {
	if (_selectedGame.expired())
		return "";

	return _selectedGame.lock()->name().c_str();
}


const char *NewWorldDialogState::selectedFormatRawName() const {
	if (_selectedFormat.expired())
		return "None";

	return _selectedFormat.lock()->info().name().c_str();
}


bool NewWorldDialogState::factoryCanBeSelected() const {
	return !_selectedGame.expired();
}

bool NewWorldDialogState::formatCanBeSelected() const {
	return !_selectedGame.expired();
}

const char *NewWorldDialogState::selectedFactoryRawName() const {
	if (_selectedFactory.expired())
		return "";

	return _selectedFactory.lock()->name().c_str();
}

void NewWorldDialogState::selectGame(std::weak_ptr<IGame> game) {
	if (game.lock() != _selectedGame.lock())
		_selectedFactory.reset();

	_selectedGame = std::move(game);
}

void NewWorldDialogState::selectFactory(std::weak_ptr<IWorldFactory> factory) {
	// TODO: game support check
	_selectedFactory = std::move(factory);
}

void NewWorldDialogState::selectFormat(std::weak_ptr<WorldFormat> format) {
	// TODO: game support check
	_selectedFormat = std::move(format);
}