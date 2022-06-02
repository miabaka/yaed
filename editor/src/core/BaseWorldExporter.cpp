#include "BaseWorldExporter.hpp"

bool BaseWorldExporter::gameIsSupported(const std::string &id) const {
	const auto &supportedIds = supportedGameIds();
	return supportedIds.find(id) != supportedIds.end();
}