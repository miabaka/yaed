#include "StubDialogProvider.hpp"
#include "StubFileDialog.hpp"

using namespace cute::dialogs;

std::unique_ptr<IFileDialog> StubDialogProvider::createFileDialog(IFileDialog::Type type) {
	return std::make_unique<StubFileDialog>();
}