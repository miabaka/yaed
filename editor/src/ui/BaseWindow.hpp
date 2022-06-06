#pragma once

#include <string>
#include "IWindow.hpp"

class BaseWindow : public IWindow {
public:
	void draw() override;

protected:
	explicit BaseWindow(std::string name);

	virtual void onDraw() = 0;

private:
	std::string _name;
	bool _open = true;
};