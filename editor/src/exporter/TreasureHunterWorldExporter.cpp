#include "TreasureHunterWorldExporter.hpp"

namespace fs = std::filesystem;

const std::string &TreasureHunterWorldExporter::name() const {
	static std::string name = "TreasureHunterWorldExporter";
	return name;
}

std::set<std::string> TreasureHunterWorldExporter::supportedGamesIds() const {
	return {"sth"};
}

void TreasureHunterWorldExporter::save(const World &world, const std::filesystem::path &path) const {

}