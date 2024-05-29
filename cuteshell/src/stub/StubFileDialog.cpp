#include "StubFileDialog.hpp"

using namespace cute::shell;

namespace fs = std::filesystem;

void StubFileDialog::addFilter(const IFileDialog::Filter &filter) {}

void StubFileDialog::clearFilters() {}

IFileDialog::ShowResult StubFileDialog::show() {
	return {};
}

void StubFileDialog::setParentWindow(window_handle_t window) {}