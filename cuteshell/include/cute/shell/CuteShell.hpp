#pragma once

#include <filesystem>
#include <memory>

#include "IDialogProvider.hpp"

namespace cute::shell {

class CuteShell {
public:
	static std::unique_ptr<IDialogProvider> createDialogProvider();

	static std::filesystem::path getAppDataPath();
};

} // namespace cute::dialogs