#include <cute/dialogs/CuteDialogs.hpp>

#include "StubDialogProvider.hpp"
#include "Win32DialogProvider.hpp"

#ifdef CUTE_DIALOGS_IMPL_GTK
#include "GtkDialogProvider.hpp"
#endif

using namespace cute::dialogs;

std::unique_ptr<IDialogProvider> CuteDialogs::createDialogProvider() {
#if defined(CUTE_DIALOGS_IMPL_WIN32)
	return std::make_unique<Win32DialogProvider>();
#elif defined(CUTE_DIALOGS_IMPL_GTK)
	return std::make_unique<GtkDialogProvider>();
#else
	return std::make_unique<StubDialogProvider>();
#endif
}