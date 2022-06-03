#pragma once

#include "../core/IWorldImporter.hpp"

class TreasureHunterWorldImporter : public IWorldImporter {
public:
	const std::string &name() const override;

	bool probe(const std::filesystem::path &path) const override;

	std::shared_ptr<World> load(
			const std::filesystem::path &path,
			const GameManager &gameManager, const WorldFactoryManager &worldFactoryManager) const override;
};