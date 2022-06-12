#pragma once

#include "../BaseWindow.hpp"

class MinimapWindow : public BaseWindow {
public:
	MinimapWindow();

protected:
	void onDraw() override;
};