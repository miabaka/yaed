#pragma once

#include "IPaletteIconProvider.hpp"

class BasePaletteIconProvider : public IPaletteIconProvider {
public:
	bool gameIsSupported(const std::string &id) const override;
};