#include "ViewportWindow.hpp"

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <glm/common.hpp>

#include "../ImUtil.hpp"

ViewportWindow::ViewportWindow()
		: BaseWindow("viewport", "Viewport", ImGuiWindowFlags_HorizontalScrollbar) {}

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

void ViewportWindow::onBeginPre() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {1, 1});
}

void ViewportWindow::onBeginPost() {
	ImGui::PopStyleVar();
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

	updateTilemapClipRect();

	{
		std::shared_ptr<BrushSelection> brushSelection;

		if (!_selectionManager.expired())
			brushSelection = _selectionManager.lock()->getSelectionForLevel(*level);

		ToolHandler::ToolUpdateParams updateParams = {
				.level = level,
				.brushSelection = brushSelection,
				.tileSize =_rendererContext->tileSize(),
		};

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