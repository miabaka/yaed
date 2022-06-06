#pragma once

#include "../BaseWindow.hpp"

class PaletteWindow : public BaseWindow {
public:
	PaletteWindow();

protected:
	void onDraw() override;
};