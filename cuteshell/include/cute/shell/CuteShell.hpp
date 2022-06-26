#pragma once

#include <filesystem>
#include <memory>

#include "IDialogProvider.hpp"

namespace cute::shell {

class CuteShell {
public:
	static std::unique_ptr<IDialogProvider> createDialogProvider();

	static std::filesystem::path getAppDataPath();

	static void revealFile(const std::filesystem::path &path);
};

} // namespace cute::dialogs