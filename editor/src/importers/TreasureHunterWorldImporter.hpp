#pragma once

#include "../IWorldImporter.hpp"

class TreasureHunterWorldImporter : public IWorldImporter {
public:
	const std::string &name() const override;

	bool probe(const std::filesystem::path &path) const override;

	std::shared_ptr<World> import(const std::filesystem::path &path) const override;

private:
	std::string _name = "TreasureHunterWorldImporter";
};