#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include "BaseGame.hpp"
#include "../LevelSkin.hpp"
#include "../LayerTemplate.hpp"
#include "../PaletteTemplate.hpp"

class KnownGame : public BaseGame {
public:
	bool known() const override;

	const std::string &id() const override;

	const std::string &name() const override;

	std::shared_ptr<const PaletteTemplate> paletteTemplate() const override;

	std::shared_ptr<const LayerTemplate> findLayerTemplateById(const std::string &id) const override;

	std::shared_ptr<LevelSkin> findLevelSkinById(int id) override;

	const std::map<int, std::shared_ptr<LevelSkin>> &levelSkins() override;

protected:
	KnownGame(std::string name, std::string id);

	void addLayerTemplate(const std::string &id, std::shared_ptr<LayerTemplate> layerTemplate);

	void addLevelSkin(std::shared_ptr<LevelSkin> levelSkin);

	void setPaletteTemplate(std::shared_ptr<PaletteTemplate> paletteTemplate);

private:
	std::string _id;
	std::string _name;
	std::unordered_map<std::string, std::shared_ptr<LayerTemplate>> _layerTemplates;
	std::map<int, std::shared_ptr<LevelSkin>> _levelSkins;
	std::shared_ptr<PaletteTemplate> _paletteTemplate;
};