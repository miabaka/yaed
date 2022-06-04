#pragma once

#include <cute/dialogs/IDialogProvider.hpp>

namespace cute::dialogs {

class Win32DialogProvider : public IDialogProvider {
public:
	Win32DialogProvider();

	std::unique_ptr<IFileDialog> createFileDialog(IFileDialog::Type type) override;
};

} // namespace cute::dialogs