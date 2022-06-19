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

void ViewportWindow::setBrushSelectionSource(std::weak_ptr<BrushSelectionManager> brushSelection) {
	_selectionManager = std::move(brushSelection);
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

	if (ImGui::IsItemHovered())
		processEdits();

	ImGui::BeginDisabled();
	ImGui::InvisibleButton("content", _rendererContext->viewportSize());
	ImGui::EndDisabled();
}

void ViewportWindow::onRender() {
	if (!_rendererContext)
		return;

	_rendererContext->render();
}

void ViewportWindow::processEdits() {
	const glm::vec2 viewportPos = ImGui::GetCursorPos();
	const glm::vec2 viewportScreenPos = ImGui::GetCursorScreenPos();

	const glm::vec2 mousePos = ImGui::GetMousePos();

	const glm::vec2 tileSize = _rendererContext->tileSize();
	const glm::ivec2 mouseTilePos = glm::floor((mousePos - viewportScreenPos) / tileSize);
	const glm::vec2 mouseTileScreenPos = viewportScreenPos + glm::vec2(mouseTilePos) * tileSize;

	{
		ImDrawList &drawList = *ImGui::GetWindowDrawList();

		drawList.AddRectFilled(
				mouseTileScreenPos, mouseTileScreenPos + tileSize, ImGui::GetColorU32(ImGuiCol_TextSelectedBg));
	}

	std::shared_ptr<Level> level = _level.lock();

	if (!level)
		return;

	std::shared_ptr<Layer> layer = level->selectedLayer();

	if (!layer)
		return;

	std::shared_ptr<BrushSelectionManager> selectionManager = _selectionManager.lock();

	if (!selectionManager)
		return;

	std::shared_ptr<BrushSelection> selection = selectionManager->getSelectionForLevel(*level);

	if (!selection)
		return;

	std::shared_ptr<Brush> activeBrush;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		activeBrush = selection->primary();
	else if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
		activeBrush = selection->secondary();

	if (!activeBrush)
		return;

	layer->tilemap().set(mouseTilePos, activeBrush->range().start());
}