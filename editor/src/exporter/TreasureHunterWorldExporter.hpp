#pragma once

#include "../core/BaseWorldExporter.hpp"

class TreasureHunterWorldExporter : public BaseWorldExporter {
	const std::string &name() const override;

	std::set<std::string> supportedGamesIds() const override;

	void save(const World &world, const std::filesystem::path &path) const override;
};