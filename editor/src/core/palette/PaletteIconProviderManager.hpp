#pragma once

#include <memory>
#include <unordered_map>

#include "../game/IGame.hpp"
#include "IPaletteIconProvider.hpp"

class PaletteIconProviderManager {
public:
	void registerProvider(std::shared_ptr<IPaletteIconProvider> provider);

	std::shared_ptr<IPaletteIconProvider> findProviderForGame(const IGame &game);

private:
	std::unordered_map<std::string, std::shared_ptr<IPaletteIconProvider>> _providers;
};