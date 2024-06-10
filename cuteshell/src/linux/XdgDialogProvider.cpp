#include "XdgDialogProvider.hpp"

#include "XdgFileDialog.hpp"

using namespace cute::shell;

std::unique_ptr<IFileDialog> XdgDialogProvider::createFileDialog(IFileDialog::Type type) {
	return std::make_unique<XdgFileDialog>(type);
}