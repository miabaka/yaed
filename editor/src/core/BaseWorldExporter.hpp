#pragma once

#include <filesystem>
#include <set>
#include "World.hpp"

class BaseWorldExporter {
public:
	virtual ~BaseWorldExporter() = default;

	virtual const std::string &name() const = 0;

	virtual std::set<std::string> supportedGamesIds() const = 0;

	bool gameIsSupported(const std::string &id) const;

	virtual void save(const World &world, const std::filesystem::path &path) const = 0;
};