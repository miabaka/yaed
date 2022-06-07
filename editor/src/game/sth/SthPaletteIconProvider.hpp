#pragma once

#include "../../core/palette/BasePaletteIconProvider.hpp"

class SthPaletteIconProvider : public BasePaletteIconProvider {
public:
	const std::string &name() const override;

	std::set<std::string> supportedGameIds() const override;

	std::shared_ptr<PaletteIconSet> getDefaultIconSetForWorld(const World &world) override;

	std::shared_ptr<PaletteIconSet> getIconSetForLevel(const Level &level) override;
};