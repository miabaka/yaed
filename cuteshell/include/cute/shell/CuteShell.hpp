#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

namespace cute::shell {

#ifdef CUTE_SHELL_LINUX
using WindowHandle = std::string;
#else
using WindowHandle = uintptr_t;
#endif

class IDialogProvider;

class CuteShell {
public:
	static std::unique_ptr<IDialogProvider> createDialogProvider();

	static std::filesystem::path getAppDataPath();

	static void revealFile(const std::filesystem::path &path);
};

} // namespace cute::dialogs