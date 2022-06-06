#pragma once

#include <string>
#include "IWindow.hpp"

class BaseWindow : public IWindow {
public:
	void draw() override;

protected:
	explicit BaseWindow(std::string name);

	virtual void onBeginPre();

	virtual void onBeginPost();

	virtual void onDraw();

private:
	std::string _name;
	bool _open = true;
};