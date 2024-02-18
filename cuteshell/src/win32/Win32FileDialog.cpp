#include "Win32FileDialog.hpp"

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

// TODO: proper conversion from utf-8
void Win32FileDialog::addExtensionFilter(std::string_view name, std::string_view extension) {
	std::wstring wideName(name.begin(), name.end());
	std::wstring wideExtension(extension.begin(), extension.end());
	_extensionFilters.emplace_back(Filter{std::move(wideName), std::move(wideExtension)});
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

	if (!SUCCEEDED(_handle->Show(nullptr)))
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