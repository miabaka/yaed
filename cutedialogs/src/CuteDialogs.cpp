#include <cute/dialogs/CuteDialogs.hpp>

#include "StubDialogProvider.hpp"
#include "Win32DialogProvider.hpp"

using namespace cute::dialogs;

std::unique_ptr<IDialogProvider> CuteDialogs::createDialogProvider() {
#ifdef CUTE_DIALOGS_IMPL_WIN32
	return std::make_unique<Win32DialogProvider>();
#else
	return std::make_unique<StubDialogProvider>();
#endif
}