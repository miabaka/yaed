#pragma once

#include "IGame.hpp"

class UnknownGame : public IGame {
public:
	explicit UnknownGame(std::string id);

	bool known() const override;

	const std::string &id() const override;

	const std::string &name() const override;

	std::shared_ptr<const LayerTemplate> findLayerTemplateById(const std::string &id) const override;

private:
	std::string _id;
};