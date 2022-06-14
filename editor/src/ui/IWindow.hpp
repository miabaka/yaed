#pragma once

class IWindow {
public:
	virtual ~IWindow() = default;

	virtual const std::string &getId() const = 0;

	virtual const std::string &getIdWithHashPrefix() const = 0;

	virtual bool &isOpen() = 0;

	virtual void setOpen(bool open) = 0;

	virtual void setTitle(const std::string &title) = 0;

	virtual void setTitle(const std::string &title, bool useAsDefault) = 0;

	virtual void restoreDefaultTitle() = 0;

	virtual void draw() = 0;

	virtual void render() = 0;
};