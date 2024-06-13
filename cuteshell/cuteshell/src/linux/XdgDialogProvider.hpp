#include "cute/shell/IDialogProvider.hpp"

namespace cute::shell {

class XdgDialogProvider : public IDialogProvider {
	std::unique_ptr<IFileDialog> createFileDialog(IFileDialog::Type type) override;
};

}