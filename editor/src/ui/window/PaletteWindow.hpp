#pragma once

#include <memory>

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

protected:
	std::weak_ptr<Level> _level;
	std::weak_ptr<LevelSkin> _levelSkin;
	std::weak_ptr<const PaletteTemplate> _template;
	std::weak_ptr<IPaletteIconProvider> _iconProvider;
	std::weak_ptr<PaletteIconSet> _iconSet;

	void onBeginPre() override;

	void onBeginPost() override;

	void onDraw() override;

private:
	void updateIconSet();
};