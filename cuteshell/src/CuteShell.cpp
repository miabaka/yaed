#include "cute/shell/CuteShell.hpp"

#include "stub/StubDialogProvider.hpp"

#ifdef CUTE_SHELL_WIN32_DIALOGS
#include "win32/Win32DialogProvider.hpp"
#endif

using namespace cute::shell;

std::unique_ptr<IDialogProvider> CuteShell::createDialogProvider() {
#ifdef CUTE_SHELL_WIN32_DIALOGS
	return std::make_unique<Win32DialogProvider>();
#else
	return std::make_unique<StubDialogProvider>();
#endif
}