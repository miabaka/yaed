#include "BaseWorldExporter.hpp"

bool BaseWorldExporter::gameIsSupported(const std::string &id) const {
	const auto &supportedIds = supportedGamesIds();
	return supportedIds.find(id) != supportedIds.end();
}