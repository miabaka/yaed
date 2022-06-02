#pragma once

#include <string>
#include <unordered_map>
#include "IGame.hpp"
#include "../LayerTemplate.hpp"

class KnownGame : public IGame {
public:
	bool known() const override;

	const std::string &id() const override;

	const std::string &name() const override;

	std::shared_ptr<const LayerTemplate> findLayerTemplateById(const std::string &id) const override;

protected:
	KnownGame(std::string name, std::string id);

	void addLayerTemplate(const std::string &id, std::shared_ptr<LayerTemplate> layerTemplate);

private:
	std::string _id;
	std::string _name;
	std::unordered_map<std::string, std::shared_ptr<LayerTemplate>> _layerTemplates;
};