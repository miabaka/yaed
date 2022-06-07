#include "PaletteWindow.hpp"

#include <imgui/imgui.h>

PaletteWindow::PaletteWindow()
		: BaseWindow("Palette") {}

void PaletteWindow::setTemplate(std::weak_ptr<const PaletteTemplate> paletteTemplate) {
	_template = std::move(paletteTemplate);
}

void PaletteWindow::setIconSet(std::weak_ptr<PaletteIconSet> iconSet) {
	_iconSet = std::move(iconSet);
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

static void drawBrush(const Brush &brush, bool &first, PaletteIconSet &iconSet) {
	if (first)
		first = false;
	else
		sameLine();

	const TextureSlice &icon = iconSet.getIconForBrush(brush);

	ImGui::ImageButton(icon.texture(), {36, 36}, icon.p1(), icon.p2(), 3);

	if (!ImGui::IsItemHovered())
		return;

	const Brush::range_t &range = brush.range();

	if (range.length() > 0)
		ImGui::SetTooltip("%s (%u-%u)", brush.name().c_str(), range.start(), range.stop());
	else
		ImGui::SetTooltip("%s (%u)", brush.name().c_str(), range.start());
}

void PaletteWindow::onDraw() {
	if (_template.expired() || _iconSet.expired())
		return;

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

		ImGui::Indent(6.f);

		bool first = true;

		for (const std::shared_ptr<Brush> &brush: group->brushes())
			drawBrush(*brush, first, *iconSet);

		ImGui::Indent(-6.f);
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(4);
}