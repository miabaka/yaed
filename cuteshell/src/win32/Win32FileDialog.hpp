#pragma once

#include <list>

#include <windows.h>
#include <shobjidl.h>
#include <wrl.h>

#include "cute/shell/IFileDialog.hpp"

namespace cute::shell {

class Win32FileDialog : public IFileDialog {
public:
	explicit Win32FileDialog(Type type);

	void addExtensionFilter(std::string_view name, std::string_view extension) override;

	void clearExtensionFilters() override;

	std::filesystem::path show() override;

	std::vector<std::filesystem::path> showForMultiple() override;

	void setParentWindow(window_handle_t window) override;

private:
	struct Filter {
		std::wstring name;
		std::wstring extension;
	};

	Microsoft::WRL::ComPtr<::IFileDialog> _handle;
	std::list<Filter> _extensionFilters;
	HWND _parentWindow = nullptr;
};

} // namespace cute::dialogs