#pragma once

#include "BaseGame.hpp"

class UnknownGame : public BaseGame {
public:
	explicit UnknownGame(std::string id);

	bool known() const override;

	const std::string &id() const override;

	const std::string &name() const override;

	std::shared_ptr<const PaletteTemplate> paletteTemplate() const override;

	std::shared_ptr<const LayerTemplate> findLayerTemplateById(const std::string &id) const override;

	std::shared_ptr<LevelSkin> findLevelSkinById(int id) override;

	const std::map<int, std::shared_ptr<LevelSkin>> &levelSkins() override;

private:
	std::string _id;
	std::string _name;
};