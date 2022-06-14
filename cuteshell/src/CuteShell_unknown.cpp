#include "cute/shell/CuteShell.hpp"

using namespace cute::shell;

namespace fs = std::filesystem;

fs::path CuteShell::getAppDataPath() {
	return fs::current_path();
}