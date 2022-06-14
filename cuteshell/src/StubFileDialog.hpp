#pragma once

#include "cute/shell/IFileDialog.hpp"

namespace cute::shell {

class StubFileDialog : public IFileDialog {
	void addExtensionFilter(const std::string &name, const std::string &extension) override;

	void addExtensionFilter(const std::wstring &name, const std::string &extension) override;

	void clearExtensionFilters() override;

	std::filesystem::path show() override;

	std::vector<std::filesystem::path> showForMultiple() override;
};

} // namespace cute::dialogs