#pragma once

#include <memory>

#include "../../core/palette/IPaletteIconProvider.hpp"
#include "../../core/palette/PaletteTemplate.hpp"
#include "../BaseWindow.hpp"

class PaletteWindow : public BaseWindow {
public:
	PaletteWindow();

	void setTemplate(std::weak_ptr<const PaletteTemplate> paletteTemplate);

	void setIconProvider(std::weak_ptr<IPaletteIconProvider> iconProvider);

protected:
	std::weak_ptr<const PaletteTemplate> _template;
	std::weak_ptr<IPaletteIconProvider> _iconProvider;

	void onBeginPre() override;

	void onBeginPost() override;

	void onDraw() override;
};