#pragma once

#include <cute/dialogs/IDialogProvider.hpp>

namespace cute::dialogs {

class StubDialogProvider : public IDialogProvider {
public:
	std::unique_ptr<IFileDialog> createFileDialog(IFileDialog::Type type) override;
};

} // namespace cute::dialogs