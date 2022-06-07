#include "BasePaletteIconProvider.hpp"

bool BasePaletteIconProvider::gameIsSupported(const std::string &id) const {
	return supportedGameIds().find(id) != supportedGameIds().end();
}