#pragma once

#include "../core/game/KnownGame.hpp"

class TreasureHunterGame : public KnownGame {
public:
	TreasureHunterGame();

private:
	void setupLayerTemplates();

	void setupPaletteTemplate();
};