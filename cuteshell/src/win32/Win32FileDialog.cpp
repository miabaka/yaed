#include "Win32FileDialog.hpp"

#include <algorithm>
#include <limits>
#include <vector>

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
			result = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, &_handle);
			break;

		case Type::Save:
			result = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, &_handle);

		default:
			break;
	}

	if (!SUCCEEDED(result))
		throw std::runtime_error(format("cuteshell Win32FileDialog: CoCreateInstance failed with {}", result));

	if (type == Type::PeekFolders)
		_handle->SetOptions(FOS_PICKFOLDERS);
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

void Win32FileDialog::addExtensionFilter(std::string_view name, std::string_view extension) {
	_extensionFilters.emplace_back(Filter{
			convertUtf8ToUtf16(name),
			convertUtf8ToUtf16(extension)
	});
}

void Win32FileDialog::clearExtensionFilters() {
	_extensionFilters.clear();
}

fs::path Win32FileDialog::show() {
	{
		std::vector<COMDLG_FILTERSPEC> fileTypes;

		fileTypes.reserve(_extensionFilters.size());

		for (const Filter &filter: _extensionFilters)
			fileTypes.emplace_back(COMDLG_FILTERSPEC{
					filter.name.c_str(),
					filter.extension.c_str()
			});

		_handle->SetFileTypes(fileTypes.size(), fileTypes.data());
	}

	if (!SUCCEEDED(_handle->Show(_parentWindow)))
		return {};

	ComPtr<IShellItem> item;

	if (!SUCCEEDED(_handle->GetResult(&item)))
		return {};

	LPWSTR itemPath;

	if (!SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &itemPath)))
		return {};

	fs::path managedPath = itemPath;
	CoTaskMemFree(itemPath);

	return managedPath;
}

std::vector<fs::path> Win32FileDialog::showForMultiple() {
	return {};
}

void Win32FileDialog::setParentWindow(window_handle_t window) {
	_parentWindow = reinterpret_cast<HWND>(window);
}