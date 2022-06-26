#pragma once

#include <set>
#include <memory>

#include "../World.hpp"
#include "../Level.hpp"
#include "PaletteIconSet.hpp"

class IPaletteIconProvider {
public:
	virtual ~IPaletteIconProvider() = default;

	virtual const std::string &name() const = 0;

	virtual std::set<std::string> supportedGameIds() const = 0;

	virtual bool gameIsSupported(const std::string &id) const = 0;

	virtual std::shared_ptr<PaletteIconSet> getDefaultIconSetForWorld(const World &world) = 0;

	virtual std::shared_ptr<PaletteIconSet> getIconSetForLevel(const Level &level) = 0;
};