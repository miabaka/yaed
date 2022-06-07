#pragma once

#include <memory>
#include "../../core/game/KnownGame.hpp"
#include "../../core/palette/PaletteTemplate.hpp"

class SthGame : public KnownGame {
public:
	SthGame();

private:
	std::shared_ptr<PaletteTemplate> _paletteTemplate;

	void setupLayerTemplates();

	void setupLevelSkins();

	void setupPaletteTemplate();
};