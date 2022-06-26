#pragma once

#include <memory>
#include <set>
#include <string>

#include "../game/IGame.hpp"
#include "ITilemapRendererContext.hpp"

class ITilemapRenderer {
public:
	virtual ~ITilemapRenderer() = default;

	virtual const std::string &name() const = 0;

	virtual std::set<std::string> supportedGameIds() const = 0;

	virtual bool gameIsSupported(const std::string &id) const = 0;

	virtual std::shared_ptr<ITilemapRendererContext> createContext(const IGame &game) = 0;
};