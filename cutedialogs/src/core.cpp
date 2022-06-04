#include <cute/dialogs/core.hpp>
#include "Win32DialogProvider.hpp"

using namespace cute::dialogs;

std::unique_ptr<IDialogProvider> cute::dialogs::createDialogProvider() {
	return std::make_unique<Win32DialogProvider>();
}