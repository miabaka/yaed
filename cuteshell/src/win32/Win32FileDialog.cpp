#include "Win32FileDialog.hpp"

#include <algorithm>
#include <limits>

#include <fmt/core.h>

using namespace cute::shell;

namespace fs = std::filesystem;

using Microsoft::WRL::ComPtr;
using fmt::format;

Win32FileDialog::Win32FileDialog(Type type) {
	HRESULT result;

	switch (type) {
		case Type::Open:
		case Type::PeekFolders:
			result = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, &_wrappedDialog);
			break;

		case Type::Save:
			result = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, &_wrappedDialog);

		default:
			break;
	}

	if (!SUCCEEDED(result))
		throw std::runtime_error(format("cuteshell Win32FileDialog: CoCreateInstance failed with {}", result));

	if (type == Type::PeekFolders)
		_wrappedDialog->SetOptions(FOS_PICKFOLDERS);
}

static std::wstring convertUtf8ToUtf16(const std::string_view src) {
	// truncate source string to avoid int overflow
	const int truncatedSrcSize = static_cast<int>(std::min(
			src.size(),
			static_cast<size_t>(std::numeric_limits<int>::max())
	));

	const int charCount = MultiByteToWideChar(CP_UTF8, 0, src.data(), truncatedSrcSize, nullptr, 0);

	if (charCount < 1)
		return {};

	std::wstring dst;
	dst.resize(charCount);

	if (MultiByteToWideChar(CP_UTF8, 0, src.data(), truncatedSrcSize, dst.data(), charCount) != charCount)
		return {};

	return dst;
}

void Win32FileDialog::addFilter(const IFileDialog::Filter &filter) {
	_filters.emplace_back(Filter{
			.name = std::string(filter.name),
			.displayName = convertUtf8ToUtf16(filter.displayName),
			.mask = convertUtf8ToUtf16(filter.mask)
	});
}

void Win32FileDialog::clearFilters() {
	_filters.clear();
}

cute::shell::IFileDialog::ShowResult Win32FileDialog::show() {
	{
		std::vector<COMDLG_FILTERSPEC> fileTypes;

		fileTypes.reserve(_filters.size());

		for (const auto &filter: _filters)
			fileTypes.emplace_back(COMDLG_FILTERSPEC{
					.pszName = filter.displayName.c_str(),
					.pszSpec = filter.mask.c_str()
			});

		_wrappedDialog->SetFileTypes(fileTypes.size(), fileTypes.data());
	}

	if (!SUCCEEDED(_wrappedDialog->Show(_parentWindow)))
		return {};

	ComPtr<IShellItem> item;

	if (!SUCCEEDED(_wrappedDialog->GetResult(&item)))
		return {};

	LPWSTR itemPath;

	if (!SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &itemPath)))
		return {};

	fs::path managedPath = itemPath;
	CoTaskMemFree(itemPath);

	return {getSelectedFilterName().value_or(""), {managedPath}};
}

void Win32FileDialog::setParentWindow(window_handle_t window) {
	_parentWindow = reinterpret_cast<HWND>(window);
}

std::optional<std::string> Win32FileDialog::getSelectedFilterName() {
	UINT filterIndex;

	if (_wrappedDialog->GetFileTypeIndex(&filterIndex) != S_OK)
		return {};

	// filterIndex initially is one-based, so subtracting one
	filterIndex--;

	if (filterIndex >= _filters.size())
		return {};

	return _filters[filterIndex].name;
}