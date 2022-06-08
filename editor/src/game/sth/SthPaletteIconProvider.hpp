#pragma once

#include <filesystem>
#include <unordered_set>

#include "../../core/palette/BasePaletteIconProvider.hpp"

class SthPaletteIconProvider : public BasePaletteIconProvider {
public:
	SthPaletteIconProvider();

	~SthPaletteIconProvider() override;

	const std::string &name() const override;

	std::set<std::string> supportedGameIds() const override;

	std::shared_ptr<PaletteIconSet> getDefaultIconSetForWorld(const World &world) override;

	std::shared_ptr<PaletteIconSet> getIconSetForLevel(const Level &level) override;

private:
	// TODO: make it independent from gapi
	std::unordered_set<unsigned int> _textureHandles;
	std::unordered_map<int, std::shared_ptr<PaletteIconSet>> _iconSets;

	void loadIconSet(const std::filesystem::path &path, int id);
};