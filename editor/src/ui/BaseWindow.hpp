#pragma once

#include <string>
#include "IWindow.hpp"

class BaseWindow : public IWindow {
public:
	bool &isOpen() override;

	void setOpen(bool open) override;

	void draw() override;

	void render() override;

	void setTitle(const std::string &title) override;

	void setTitle(const std::string &title, bool useAsDefault) override;

	void restoreDefaultTitle() override;

protected:
	explicit BaseWindow(const std::string &id, int flags);

	explicit BaseWindow(const std::string &id, const std::string &title = {}, int flags = 0);

	virtual void onBeginPre();

	virtual void onBeginPost();

	virtual void onDraw();

	virtual void onRender();

private:
	std::string _id;
	std::string _actualTitle;
	std::string _defaultTitle;
	int _flags;
	bool _open = true;
};