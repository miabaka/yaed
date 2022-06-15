#pragma once

#include <memory>

#include "../../core/palette/BrushSelectionManager.hpp"
#include "../../core/palette/IPaletteIconProvider.hpp"
#include "../../core/palette/PaletteIconSet.hpp"
#include "../../core/palette/PaletteTemplate.hpp"
#include "../../core/BaseEditor.hpp"
#include "../../core/Level.hpp"
#include "../BaseWindow.hpp"

class PaletteWindow : public BaseWindow {
public:
	PaletteWindow();

	void setLevel(BaseEditor &editor, std::shared_ptr<Level> level);

	void setBrushSelectionSource(std::weak_ptr<BrushSelectionManager> brushSelection);

protected:
	void onBeginPre() override;

	void onBeginPost() override;

	void onDraw() override;

private:
	std::weak_ptr<Level> _level;
	std::weak_ptr<LevelSkin> _levelSkin;
	std::weak_ptr<const PaletteTemplate> _template;
	std::weak_ptr<IPaletteIconProvider> _iconProvider;
	std::weak_ptr<PaletteIconSet> _iconSet;
	std::weak_ptr<BrushSelectionManager> _selectionManager;

	void updateIconSet();

	void
	drawBrush(std::shared_ptr<Brush> brush, bool allowed, PaletteIconSet &iconSet, std::shared_ptr<BrushSelection> selection);
};