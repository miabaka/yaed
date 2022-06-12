#pragma once

class IWindow {
public:
	virtual ~IWindow() = default;

	virtual void setTitle(const std::string &title) = 0;

	virtual void setTitle(const std::string &title, bool useAsDefault) = 0;

	virtual void restoreDefaultTitle() = 0;

	virtual void draw() = 0;

	virtual void render() = 0;
};