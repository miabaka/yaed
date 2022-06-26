#pragma once

#include <filesystem>
#include <set>
#include "../World.hpp"

class IWorldExporter {
public:
	virtual ~IWorldExporter() = default;

	virtual const std::string &name() const = 0;

	virtual std::set<std::string> supportedGameIds() const = 0;

	virtual bool gameIsSupported(const std::string &id) const = 0;

	virtual void save(const World &world, const std::filesystem::path &path) const = 0;
};