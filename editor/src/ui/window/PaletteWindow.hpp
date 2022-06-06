#pragma once

#include <memory>

#include "../../core/PaletteTemplate.hpp"
#include "../BaseWindow.hpp"

class PaletteWindow : public BaseWindow {
public:
	PaletteWindow();

	void setTemplate(std::weak_ptr<const PaletteTemplate> paletteTemplate);

protected:
	std::weak_ptr<const PaletteTemplate> _template;

	void onDraw() override;
};