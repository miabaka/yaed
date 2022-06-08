#pragma once

#include <string>
#include "IWindow.hpp"

class BaseWindow : public IWindow {
public:
	void draw() override;

	void setTitle(const std::string &title) override;

	void setTitle(const std::string &title, bool useAsDefault) override;

	void restoreDefaultTitle() override;

protected:
	explicit BaseWindow(const std::string &id, const std::string &title = {});

	virtual void onBeginPre();

	virtual void onBeginPost();

	virtual void onDraw();

private:
	std::string _id;
	std::string _actualTitle;
	std::string _defaultTitle;
	bool _open = true;
};