#pragma once

#include <memory>
#include "IDialogProvider.hpp"

namespace cute::shell {

class CuteShell {
public:
	static std::unique_ptr<IDialogProvider> createDialogProvider();
};

} // namespace cute::dialogs