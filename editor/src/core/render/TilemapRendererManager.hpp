#pragma once

#include <unordered_map>
#include <memory>

#include "../game/IGame.hpp"
#include "ITilemapRenderer.hpp"

class TilemapRendererManager {
public:
	void registerRenderer(std::shared_ptr<ITilemapRenderer> renderer);

	std::shared_ptr<ITilemapRenderer> findRendererForGame(const IGame &game);

private:
	std::unordered_map<std::string, std::shared_ptr<ITilemapRenderer>> _renderers;
};