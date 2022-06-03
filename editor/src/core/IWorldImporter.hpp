#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "World.hpp"
#include "WorldFactoryManager.hpp"

class IWorldImporter {
public:
	virtual ~IWorldImporter() = default;

	virtual const std::string &name() const = 0;

	virtual bool probe(const std::filesystem::path &path) const = 0;

	virtual std::shared_ptr<World> load(
			const std::filesystem::path &path,
			const GameManager &gameManager, const WorldFactoryManager &worldFactoryManager) const = 0;
};