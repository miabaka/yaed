#include "SthTilemapRenderer.hpp"

#include "SthTilemapRendererContext.hpp"

SthTilemapRenderer::SthTilemapRenderer()
		: _internal(std::make_shared<SthTilemapRendererInternal>()) {}

const std::string &SthTilemapRenderer::name() const {
	static std::string name = "SthTilemapRenderer";
	return name;
}

std::set<std::string> SthTilemapRenderer::supportedGameIds() const {
	return {"sth"};
}

std::shared_ptr<ITilemapRendererContext> SthTilemapRenderer::createContext(const IGame &game) {
	auto context = new SthTilemapRendererContext(_internal);
	return std::shared_ptr<ITilemapRendererContext>(context);
}