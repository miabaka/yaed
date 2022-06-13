#pragma once

#include <memory>

#include "../../core/Level.hpp"
#include "../BaseWindow.hpp"

class LayerListWindow : public BaseWindow {
public:
	LayerListWindow();

	void setLevel(std::weak_ptr<Level> level);

protected:
	void onBeginPre() override;

	void onBeginPost() override;

	void onDraw() override;

private:
	std::weak_ptr<Level> _level;
};