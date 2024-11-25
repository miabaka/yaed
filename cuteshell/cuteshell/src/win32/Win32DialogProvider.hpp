#pragma once

#include "cute/shell/IDialogProvider.hpp"

namespace cute::shell {

class Win32DialogProvider : public IDialogProvider {
public:
	Win32DialogProvider();

	~Win32DialogProvider() override;

	std::unique_ptr<IFileDialog> createFileDialog(IFileDialog::Type type) override;
};

} // namespace cute::dialogs