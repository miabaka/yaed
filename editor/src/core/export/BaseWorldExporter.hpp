#pragma once

#include "IWorldExporter.hpp"

class BaseWorldExporter : public IWorldExporter {
public:
	bool gameIsSupported(const std::string &id) const override;
};