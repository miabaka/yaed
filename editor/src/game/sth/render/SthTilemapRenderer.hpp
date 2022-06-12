#pragma once

#include <memory>

#include "../../../core/render/BaseTilemapRenderer.hpp"
#include "SthTilemapRendererInternal.hpp"

class SthTilemapRenderer : public BaseTilemapRenderer {
public:
	SthTilemapRenderer();

	const std::string &name() const override;

	std::set<std::string> supportedGameIds() const override;

	std::shared_ptr<ITilemapRendererContext> createContext(const IGame &game) override;

private:
	std::shared_ptr<SthTilemapRendererInternal> _internal;
};