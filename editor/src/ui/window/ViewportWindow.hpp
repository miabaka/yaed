#pragma once

#include "../../core/Level.hpp"
#include "../BaseWindow.hpp"

class ViewportWindow : public BaseWindow {
public:
	ViewportWindow();

	void setLevel(std::weak_ptr<Level> level);

protected:
	void onDraw() override;

private:
	std::weak_ptr<Level> _level;
};