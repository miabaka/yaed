#pragma once

#include <filesystem>
#include <memory>

#include "IFileDialog.hpp"

namespace cute::shell {

class IDialogProvider {
public:
	virtual ~IDialogProvider() = default;

	virtual std::unique_ptr<IFileDialog> createFileDialog(IFileDialog::Type type) = 0;
};

} // namespace cute::dialogs