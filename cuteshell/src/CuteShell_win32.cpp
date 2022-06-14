#include "cute/shell/CuteShell.hpp"

#include <windows.h>
#include <shlobj.h>

using namespace cute::shell;

namespace fs = std::filesystem;

fs::path CuteShell::getAppDataPath() {
	PWSTR path;

	if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path) != S_OK)
		return {};

	fs::path managedPath = path;
	CoTaskMemFree(path);

	return managedPath;
}