#pragma once

#include "ITilemapRenderer.hpp"

class BaseTilemapRenderer : public ITilemapRenderer {
public:
	bool gameIsSupported(const std::string &id) const override;
};