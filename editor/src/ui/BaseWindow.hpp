#pragma once

#include <string>
#include <glm/vec2.hpp>
#include "IWindow.hpp"

class BaseWindow : public IWindow {
public:
	const std::string &getId() const override;

	const std::string &getIdWithHashPrefix() const override;

	bool &isOpen() override;

	void setOpen(bool open) override;

	void draw() override;

	void render() override;

	void setTitle(const std::string &title) override;

	void setTitle(const std::string &title, bool useAsDefault) override;

	void setPadding(glm::vec2 padding) override;

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
	std::string _idWithHashPrefix;
	std::string _actualTitle;
	std::string _defaultTitle;
	int _flags;
	bool _open = true;
	bool _previouslyDocked = false;
	glm::vec2 _padding = {6, 6};
};