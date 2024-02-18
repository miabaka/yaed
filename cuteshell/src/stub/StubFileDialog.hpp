#pragma once

#include "cute/shell/IFileDialog.hpp"

namespace cute::shell {

class StubFileDialog : public IFileDialog {
	void addExtensionFilter(std::string_view name, std::string_view extension) override;

	void clearExtensionFilters() override;

	std::filesystem::path show() override;

	std::vector<std::filesystem::path> showForMultiple() override;

	void setParentWindow(window_handle_t window) override;
};

} // namespace cute::dialogs