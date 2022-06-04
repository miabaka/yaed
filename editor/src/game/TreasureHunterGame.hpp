#pragma once

#include <memory>
#include "../core/game/KnownGame.hpp"
#include "../core/PaletteTemplate.hpp"

class TreasureHunterGame : public KnownGame {
public:
	TreasureHunterGame();

private:
	std::shared_ptr<PaletteTemplate> _paletteTemplate;

	void setupLayerTemplates();

	void setupLevelSkins();

	void setupPaletteTemplate();
};