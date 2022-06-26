#include "cute/shell/CuteShell.hpp"

using namespace cute::shell;

namespace fs = std::filesystem;

fs::path CuteShell::getAppDataPath() {
	return fs::current_path();
}

void CuteShell::revealFile(const std::filesystem::path &path) {}