#pragma once

#include "cute/shell/IFileDialog.hpp"

namespace cute::shell {

class StubFileDialog : public IFileDialog {
	void addFilter(const Filter &filter) override;

	void clearFilters() override;

	ShowResult show() override;

	void setParentWindow(WindowHandle window) override;
};

} // namespace cute::dialogs