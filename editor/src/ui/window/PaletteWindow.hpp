#pragma once

#include <memory>

#include "../../core/palette/PaletteIconSet.hpp"
#include "../../core/palette/PaletteTemplate.hpp"
#include "../BaseWindow.hpp"

class PaletteWindow : public BaseWindow {
public:
	PaletteWindow();

	void setTemplate(std::weak_ptr<const PaletteTemplate> paletteTemplate);

	void setIconSet(std::weak_ptr<PaletteIconSet> iconSet);

protected:
	std::weak_ptr<const PaletteTemplate> _template;
	std::weak_ptr<PaletteIconSet> _iconSet;

	void onBeginPre() override;

	void onBeginPost() override;

	void onDraw() override;
};