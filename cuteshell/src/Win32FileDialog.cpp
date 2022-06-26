#include "Win32FileDialog.hpp"

#include <vector>

#include <windows.h>
#include <shobjidl.h>

using namespace cute::shell;

namespace fs = std::filesystem;

Win32FileDialog::Win32FileDialog(Type type)
		: _type(type),
		  _handle() {
	switch (type) {
		case Type::Open:
		case Type::PeekFolders:
			CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, &_handle);
			break;

		case Type::Save:
			CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, &_handle);

		default:
			break;
	}

	if (type == Type::PeekFolders)
		static_cast<::IFileDialog *>(_handle)->SetOptions(FOS_PICKFOLDERS);
}

Win32FileDialog::~Win32FileDialog() {
	static_cast<::IFileDialog *>(_handle)->Release();
}

void Win32FileDialog::addExtensionFilter(const std::string &name, const std::string &extension) {
	std::wstring wideName(name.begin(), name.end());
	std::wstring wideExtension(extension.begin(), extension.end());
	_extensionFilters.emplace_back(Filter{std::move(wideName), std::move(wideExtension)});
}

void Win32FileDialog::addExtensionFilter(const std::wstring &name, const std::string &extension) {
	std::wstring wideExtension(extension.begin(), extension.end());
	_extensionFilters.emplace_back(Filter{name, std::move(wideExtension)});
}

void Win32FileDialog::clearExtensionFilters() {
	_extensionFilters.clear();
}

// TODO: proper error handling
fs::path Win32FileDialog::show() {
	auto *dialog = static_cast<::IFileDialog *>(_handle);

	std::vector<COMDLG_FILTERSPEC> extensionFilters;

	extensionFilters.reserve(_extensionFilters.size());

	for (const Filter &filter: _extensionFilters)
		extensionFilters.emplace_back(COMDLG_FILTERSPEC{
				filter.name.c_str(),
				filter.extension.c_str()
		});

	dialog->SetFileTypes(extensionFilters.size(), extensionFilters.data());

	if (FAILED(dialog->Show(nullptr)))
		return {};

	fs::path result;

	IShellItem *item;
	dialog->GetResult(&item);

	wchar_t *itemPath;
	item->GetDisplayName(SIGDN_FILESYSPATH, &itemPath);

	result = itemPath;

	item->Release();

	return result;
}

std::vector<fs::path> Win32FileDialog::showForMultiple() {
	return {};
}