#pragma once

#include <map>

#include "../palette/PaletteTemplate.hpp"
#include "../LayerTemplate.hpp"
#include "../LevelSkin.hpp"

class IGame {
public:
	virtual ~IGame() = default;

	virtual bool known() const = 0;

	virtual const std::string &id() const = 0;

	virtual const std::string &name() const = 0;

	virtual std::shared_ptr<const PaletteTemplate> paletteTemplate() const = 0;

	virtual std::shared_ptr<const LayerTemplate> findLayerTemplateById(const std::string &id) const = 0;

	virtual std::shared_ptr<LevelSkin> findLevelSkinById(int id) = 0;

	virtual const std::map<int, std::shared_ptr<LevelSkin>> &levelSkins() = 0;

	virtual std::shared_ptr<LevelSkin> defaultLevelSkin() = 0;
};