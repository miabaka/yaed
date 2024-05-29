#pragma once

#include <memory>
#include <string>

#include "../core/game/IGame.hpp"
#include "../core/IWorldFactory.hpp"

class NewWorldDialogState {
public:
	void reset();

	bool valid() const;

	std::string &chosenName();

	const std::string &chosenName() const;

	const char *selectedGameRawName() const;

	const char *selectedFactoryRawName() const;

	std::shared_ptr<IGame> selectedGame() const;

	std::shared_ptr<IWorldFactory> selectedFactory() const;

	bool factoryCanBeSelected() const;

	void selectGame(std::weak_ptr<IGame> game);

	void selectFactory(std::weak_ptr<IWorldFactory> factory);

private:
	std::string _chosenName;
	std::weak_ptr<IGame> _selectedGame;
	std::weak_ptr<IWorldFactory> _selectedFactory;
};