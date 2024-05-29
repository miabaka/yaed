#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <optional>
#include <vector>

#include <windows.h>
#include <shobjidl.h>
#include <wrl.h>

#include "cute/shell/IFileDialog.hpp"

namespace cute::shell {

class Win32FileDialog : public IFileDialog {
public:
	explicit Win32FileDialog(Type type);

	void addFilter(const Filter &filter) override;

	void clearFilters() override;

	ShowResult show() override;

	void setParentWindow(window_handle_t window) override;

private:
	struct Filter {
		std::string name;
		std::wstring displayName;
		std::wstring mask;
	};

	Microsoft::WRL::ComPtr<::IFileDialog> _wrappedDialog;
	std::vector<Filter> _filters;
	HWND _parentWindow = nullptr;

	std::optional<std::string> getSelectedFilterName();
};

} // namespace cute::dialogs