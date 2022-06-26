#include "BaseTilemapRenderer.hpp"

bool BaseTilemapRenderer::gameIsSupported(const std::string &id) const {
	const auto &supportedIds = supportedGameIds();
	return supportedIds.find(id) != supportedIds.end();
}