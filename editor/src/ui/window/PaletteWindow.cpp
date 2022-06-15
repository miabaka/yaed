#include "PaletteWindow.hpp"

#include <imgui/imgui.h>

enum class BrushSelectionState {
	None = 0,
	Primary,
	Secondary,
	Both
};

PaletteWindow::PaletteWindow()
		: BaseWindow("palette", "Palette") {}

void PaletteWindow::setLevel(BaseEditor &editor, std::shared_ptr<Level> level) {
	_level = level;

	if (!level) {
		_template = {};
		_iconProvider = {};
		return;
	}

	const IGame &game = *level->world()->game();

	_iconProvider = editor.paletteIconProviders().findProviderForGame(game);
	_template = game.paletteTemplate();
}

void PaletteWindow::setBrushSelectionSource(std::weak_ptr<BrushSelectionManager> brushSelection) {
	_selectionManager = std::move(brushSelection);
}

void PaletteWindow::onBeginPre() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {6, 6});
}

void PaletteWindow::onBeginPost() {
	ImGui::PopStyleVar();
}

static void sameLine() {
	ImGui::SameLine();

	if ((ImGui::GetCursorPosX() + 42) > ImGui::GetWindowContentRegionMax().x)
		ImGui::NewLine();
}

void PaletteWindow::onDraw() {
	if (_level.expired() || _template.expired())
		return;

	updateIconSet();

	// TODO: use default stub icon set in such situation
	if (_iconSet.expired()) {
		ImGui::TextWrapped("Can't show: icon set is a hiding baka!");
		return;
	}

	std::shared_ptr<Level> level = _level.lock();
	std::shared_ptr<const LayerTemplate> layerTemplate;

	if (!level->selectedLayer()) {
		ImGui::TextWrapped("Can't show: no selected layer");
		return;
	}

	layerTemplate = level->selectedLayer()->getTemplate();

	if (!layerTemplate) {
		ImGui::TextWrapped("Can't show: layer template isn't set");
		return;
	}

	const std::unordered_set<Tilemap::tile_t> allowedTiles = layerTemplate->allowedTiles();

	std::shared_ptr<PaletteIconSet> iconSet = _iconSet.lock();
	std::shared_ptr<const PaletteTemplate> palTemplate = _template.lock();

	std::shared_ptr<BrushSelection> selection;

	if (!_selectionManager.expired())
		selection = _selectionManager.lock()->getSelectionForLevel(*level, *palTemplate);

	ImGui::PushStyleColor(ImGuiCol_Header, 0);
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, 0);
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, 0);
	ImGui::PushStyleColor(ImGuiCol_Button, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

	const int headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

	for (const std::shared_ptr<BrushGroup> &group: palTemplate->brushGroups()) {
		if (!ImGui::CollapsingHeader(group->name().c_str(), headerFlags))
			continue;

		ImGui::Indent(6);

		bool first = true;

		for (const std::shared_ptr<Brush> &brush: group->brushes()) {
			if (first)
				first = false;
			else
				sameLine();

			const bool allowed = allowedTiles.find(brush->range().start()) != allowedTiles.end();

			drawBrush(brush, allowed, *iconSet, selection);
		}

		ImGui::Indent(-6);
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(4);
}

void PaletteWindow::updateIconSet() {
	if (_level.expired() || _iconProvider.expired())
		return;

	std::shared_ptr<Level> level = _level.lock();
	std::shared_ptr<LevelSkin> levelSkin = level->skin();

	if (_levelSkin.lock() == levelSkin)
		return;

	_levelSkin = levelSkin;

	_iconSet = _iconProvider.lock()->getIconSetForLevel(*level);
}

// TODO: make it look not that awful
static void drawBrushSlot(glm::vec2 initialPos, glm::vec2 fullButtonSize, BrushSelectionState selection) {
	const char *const labels[] = {nullptr, "1", "2", "1+2"};
	const char *label;

	{
		const auto labelIndex = static_cast<int>(selection);

		if (!(labelIndex > 0 && labelIndex < std::size(labels)))
			return;

		label = labels[labelIndex];
	}

	ImDrawList &drawList = *ImGui::GetWindowDrawList();

	// carefully picked by eye for the default font
	const glm::vec2 textOffset = {-5, -4};
	const glm::vec2 leftTopCornerOffset = {-5, -3};

	const glm::vec2 textSize = ImGui::CalcTextSize(label);

	const glm::vec2 rightBottomCorner = initialPos + fullButtonSize;
	const glm::vec2 textPos = rightBottomCorner - textSize + textOffset;
	const glm::vec2 leftTopCorner = textPos + leftTopCornerOffset;

	drawList.AddRectFilled(leftTopCorner, rightBottomCorner, ImGui::GetColorU32(ImGuiCol_TextSelectedBg));
	drawList.AddText(textPos, 0xff << 24, label);
}

static BrushSelectionState getSelectionStateForBrush(
		const Brush &brush, const std::shared_ptr<BrushSelection> &selection) {
	if (!selection)
		return BrushSelectionState::None;

	const bool primary = (&brush == selection->primary().get());
	const bool secondary = (&brush == selection->secondary().get());

	return static_cast<BrushSelectionState>((secondary << 1) | primary);
}

void PaletteWindow::drawBrush(
		std::shared_ptr<Brush> brush, bool allowed, PaletteIconSet &iconSet,
		std::shared_ptr<BrushSelection> selection) {
	const BrushSelectionState selectionState = getSelectionStateForBrush(*brush, selection);

	const int buttonPadding = 3;
	constexpr glm::vec2 baseButtonSize = {36, 36};
	constexpr glm::vec2 fullButtonSize = baseButtonSize + float(buttonPadding * 2);

	const glm::vec2 screenPos = ImGui::GetCursorScreenPos();

	const TextureSlice &icon = iconSet.getIconForBrush(*brush);

	if (selectionState != BrushSelectionState::None)
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_TextSelectedBg));

	ImGui::BeginDisabled(!allowed);
	ImGui::ImageButton(icon.texture(), baseButtonSize, icon.p1(), icon.p2(), buttonPadding);
	ImGui::EndDisabled();

	if (selectionState != BrushSelectionState::None)
		ImGui::PopStyleColor();

	if (selection) {
		// TODO: change selection on mouse up instead of mouse down
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			selection->setPrimary(brush);
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			selection->setSecondary(brush);
	}

	drawBrushSlot(screenPos, fullButtonSize, selectionState);

	if (!ImGui::IsItemHovered())
		return;

	const Brush::range_t &range = brush->range();

	if (range.length() > 0)
		ImGui::SetTooltip("%s (%u-%u)", brush->name().c_str(), range.start(), range.stop());
	else
		ImGui::SetTooltip("%s (%u)", brush->name().c_str(), range.start());
}