#include "cute/shell/CuteShell.hpp"

#include "stub/StubDialogProvider.hpp"

#ifdef CUTE_SHELL_WIN32_DIALOGS
#include "win32/Win32DialogProvider.hpp"
#endif

#ifdef CUTE_SHELL_XDG_DIALOGS
#include "linux/XdgDialogProvider.hpp"
#endif

using namespace cute::shell;

std::unique_ptr<IDialogProvider> CuteShell::createDialogProvider() {
#ifdef CUTE_SHELL_WIN32_DIALOGS
	return std::make_unique<Win32DialogProvider>();
#elif defined(CUTE_SHELL_XDG_DIALOGS)
	return std::make_unique<XdgDialogProvider>();
#else
	return std::make_unique<StubDialogProvider>();
#endif
}