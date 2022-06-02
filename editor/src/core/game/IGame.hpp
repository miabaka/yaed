#pragma once

#include <memory>
#include <string>

#include "../LayerTemplate.hpp"

class IGame {
public:
	virtual ~IGame() = default;

	virtual bool known() const = 0;

	virtual const std::string &id() const = 0;

	virtual const std::string &name() const = 0;

	virtual std::shared_ptr<const LayerTemplate> findLayerTemplateById(const std::string &id) const = 0;
};