#pragma once

#include <filesystem>
#include <memory>

namespace cute::shell {

using window_handle_t = void *;

class IDialogProvider;

class CuteShell {
public:
	static std::unique_ptr<IDialogProvider> createDialogProvider();

	static std::filesystem::path getAppDataPath();

	static void revealFile(const std::filesystem::path &path);
};

} // namespace cute::dialogs