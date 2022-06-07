#include "SthPaletteIconProvider.hpp"

const std::string &SthPaletteIconProvider::name() const {
	static std::string name = "SthPaletteIconProvider";
	return name;
}

std::set<std::string> SthPaletteIconProvider::supportedGameIds() const {
	return {"sth"};
}

std::shared_ptr<PaletteIconSet> SthPaletteIconProvider::getDefaultIconSetForWorld(const World &world) {
	return {};
}

std::shared_ptr<PaletteIconSet> SthPaletteIconProvider::getIconSetForLevel(const Level &level) {
	return {};
}