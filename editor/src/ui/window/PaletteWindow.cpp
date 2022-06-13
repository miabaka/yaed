#include "PaletteWindow.hpp"

#include <imgui/imgui.h>

PaletteWindow::PaletteWindow()
		: BaseWindow("Palette") {}

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

static void drawBrush(const Brush &brush, bool &first, bool allowed, PaletteIconSet &iconSet) {
	if (first)
		first = false;
	else
		sameLine();

	const TextureSlice &icon = iconSet.getIconForBrush(brush);

	ImGui::BeginDisabled(!allowed);

	ImGui::ImageButton(icon.texture(), {36, 36}, icon.p1(), icon.p2(), 3);

	ImGui::EndDisabled();

	if (!ImGui::IsItemHovered())
		return;

	const Brush::range_t &range = brush.range();

	if (range.length() > 0)
		ImGui::SetTooltip("%s (%u-%u)", brush.name().c_str(), range.start(), range.stop());
	else
		ImGui::SetTooltip("%s (%u)", brush.name().c_str(), range.start());
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
			const bool allowed = allowedTiles.find(brush->range().start()) != allowedTiles.end();
			drawBrush(*brush, first, allowed, *iconSet);
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