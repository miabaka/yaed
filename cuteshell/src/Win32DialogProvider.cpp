#include "Win32DialogProvider.hpp"

#include <windows.h>
#include "Win32FileDialog.hpp"

using namespace cute::shell;

Win32DialogProvider::Win32DialogProvider() {
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

std::unique_ptr<IFileDialog> Win32DialogProvider::createFileDialog(IFileDialog::Type type) {
	return std::make_unique<Win32FileDialog>(type);
}