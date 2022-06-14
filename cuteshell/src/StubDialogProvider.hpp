#pragma once

#include "cute/shell/IDialogProvider.hpp"

namespace cute::shell {

class StubDialogProvider : public IDialogProvider {
public:
	std::unique_ptr<IFileDialog> createFileDialog(IFileDialog::Type type) override;
};

} // namespace cute::dialogs