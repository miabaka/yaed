#include "PaletteIconProviderManager.hpp"

#include <stdexcept>
#include <fmt/core.h>

void PaletteIconProviderManager::registerProvider(std::shared_ptr<IPaletteIconProvider> provider) {
	using std::runtime_error;

	for (const std::string &gameId: provider->supportedGameIds()) {
		if (_providers.find(gameId) != _providers.end())
			throw runtime_error(fmt::format("Palette icon provider for game id '{}' is already registered", gameId));

		_providers[gameId] = provider;
	}
}

std::shared_ptr<IPaletteIconProvider> PaletteIconProviderManager::findProviderForGame(const IGame &game) {
	const auto it = _providers.find(game.id());

	if (it == _providers.end())
		return {};

	return it->second;
}