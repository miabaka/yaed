#include "TilemapRendererManager.hpp"

#include <stdexcept>
#include <fmt/format.h>

void TilemapRendererManager::registerRenderer(std::shared_ptr<ITilemapRenderer> renderer) {
	using fmt::format;
	using std::runtime_error;

	for (const std::string &gameId: renderer->supportedGameIds()) {
		if (_renderers.find(gameId) != _renderers.end())
			throw runtime_error(format("Palette icon provider for game id '{}' is already registered", gameId));

		_renderers[gameId] = renderer;
	}
}

std::shared_ptr<ITilemapRenderer> TilemapRendererManager::findRendererForGame(const IGame &game) {
	const auto it = _renderers.find(game.id());

	if (it == _renderers.end())
		return {};

	return it->second;
}