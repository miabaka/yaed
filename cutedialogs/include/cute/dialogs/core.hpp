#pragma once

#include <memory>
#include "IDialogProvider.hpp"

namespace cute::dialogs {

std::unique_ptr<IDialogProvider> createDialogProvider();

} // namespace cute::dialogs