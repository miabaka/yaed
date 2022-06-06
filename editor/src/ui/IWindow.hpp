#pragma once

class IWindow {
public:
	virtual ~IWindow() = default;

	virtual void draw() = 0;
};