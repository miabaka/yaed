#include "cute/shell/CuteShell.hpp"

#include "stub/StubDialogProvider.hpp"

#ifdef CUTE_SHELL_WIN32_IMPL
#include "win32/Win32DialogProvider.hpp"
#endif

using namespace cute::shell;

std::unique_ptr<IDialogProvider> CuteShell::createDialogProvider() {
#ifdef CUTE_SHELL_WIN32_IMPL
	return std::make_unique<Win32DialogProvider>();
#else
	return std::make_unique<StubDialogProvider>();
#endif
}