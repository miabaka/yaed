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

void CuteShell::revealFile(const fs::path &path) {
	PIDLIST_ABSOLUTE itemIds;
	SFGAOF flags;

	if (SHParseDisplayName(path.wstring().c_str(), nullptr, &itemIds, 0, &flags) != S_OK)
		return;

	SHOpenFolderAndSelectItems(itemIds, 0, nullptr, 0);

	CoTaskMemFree(itemIds);
}