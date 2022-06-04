#include "StubFileDialog.hpp"

using namespace cute::dialogs;

namespace fs = std::filesystem;

void StubFileDialog::addExtensionFilter(const std::string &name, const std::string &extension) {}

void StubFileDialog::addExtensionFilter(const std::wstring &name, const std::string &extension) {}

void StubFileDialog::clearExtensionFilters() {}

fs::path StubFileDialog::show() {
	return {};
}

fs::path StubFileDialog::showForMultiple() {
	return {};
}