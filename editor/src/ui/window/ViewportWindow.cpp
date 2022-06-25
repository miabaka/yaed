#include "ViewportWindow.hpp"

#include <fmt/format.h>
#include <imgui/imgui.h>

#include "../ImUtil.hpp"

ViewportWindow::ViewportWindow()
		: BaseWindow("viewport", "Viewport", ImGuiWindowFlags_HorizontalScrollbar) {
	BaseWindow::setPadding({1, 1});
}

void ViewportWindow::setLevel(BaseEditor &editor, std::shared_ptr<Level> level) {
	_level = level;

	if (!level) {
		restoreDefaultTitle();
		return;
	}

	const IGame &game = *level->world()->game();

	if (!(_rendererContext && _rendererContext->compatibleWith(game))) {
		std::shared_ptr<ITilemapRenderer> renderer =
				editor.tilemapRenderers().findRendererForGame(game);

		_rendererContext = renderer->createContext(game);
	}

	if (!_rendererContext)
		return;

	_rendererContext->setLevel(level);
}

void ViewportWindow::setBrushSelectionSource(std::weak_ptr<BrushSelectionManager> selectionManager) {
	_selectionManager = std::move(selectionManager);
}

void ViewportWindow::onDraw() {
	if (_level.expired())
		return;

	std::shared_ptr<Level> level = _level.lock();

	// TODO: move this line to some kind of rename event handler
	setTitle(fmt::format("{} - {}", level->world()->nameOrFilename(), level->name()));

	if (!_rendererContext) {
		ImUtil::centeredText("Can't show: renderer context is a *hiding* baka!");
		return;
	}

	ImUtil::centeredImage(_rendererContext->viewportTexture(), _rendererContext->viewportSize(), {0, 1}, {1, 0});
	drawLayerBounds();

	updateTilemapClipRect();

	{
		std::shared_ptr<BrushSelection> brushSelection;

		if (!_selectionManager.expired())
			brushSelection = _selectionManager.lock()->getSelectionForLevel(*level);

		ToolHandler::ToolUpdateParams updateParams{};

		updateParams.level = level;
		updateParams.brushSelection = brushSelection;
		updateParams.tileSize = _rendererContext->tileSize();

		_toolHandler.update(updateParams);
	}

	ImGui::BeginDisabled();
	ImGui::InvisibleButton("content", _rendererContext->viewportSize());
	ImGui::EndDisabled();
}

void ViewportWindow::onRender() {
	if (!_rendererContext)
		return;

	_rendererContext->render();
}

void ViewportWindow::updateTilemapClipRect() {

}

void ViewportWindow::drawLayerBounds() {
	if (!_rendererContext)
		return;

	std::shared_ptr<Level> level = _level.lock();

	if (!level)
		return;

	std::shared_ptr<Layer> layer = level->selectedLayer();

	if (!layer)
		return;

	const IntRect region = layer->tilemap().occupiedRegion();

	const glm::vec2 offset = ImGui::GetCursorScreenPos();
	const glm::vec2 tileSize = _rendererContext->tileSize();

	const glm::vec2 p0 = glm::vec2(region.p0()) * tileSize + offset;
	const glm::vec2 p1 = glm::vec2(region.p1()) * tileSize + offset;

	ImGui::GetWindowDrawList()->AddRect(p0, p1, ImGui::GetColorU32(ImGuiCol_TextSelectedBg));
}