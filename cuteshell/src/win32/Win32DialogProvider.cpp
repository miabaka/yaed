#include "Win32DialogProvider.hpp"

#include <stdexcept>
#include <windows.h>
#include "Win32FileDialog.hpp"

using namespace cute::shell;

Win32DialogProvider::Win32DialogProvider() {
	if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE)))
		throw std::runtime_error("cuteshell: Failed to initialize COM");
}

Win32DialogProvider::~Win32DialogProvider() {
	CoUninitialize();
}

std::unique_ptr<cute::shell::IFileDialog> Win32DialogProvider::createFileDialog(IFileDialog::Type type) {
	return std::make_unique<Win32FileDialog>(type);
}