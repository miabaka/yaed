#pragma once

#include <memory>
#include "IDialogProvider.hpp"

namespace cute::dialogs {

class CuteDialogs {
public:
	static std::unique_ptr<IDialogProvider> createDialogProvider();
};

} // namespace cute::dialogs