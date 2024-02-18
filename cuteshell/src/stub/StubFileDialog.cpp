#include "StubFileDialog.hpp"

using namespace cute::shell;

namespace fs = std::filesystem;

void StubFileDialog::addExtensionFilter(std::string_view name, std::string_view extension) {}

void StubFileDialog::clearExtensionFilters() {}

fs::path StubFileDialog::show() {
	return {};
}

std::vector<fs::path> StubFileDialog::showForMultiple() {
	return {};
}

void StubFileDialog::setParentWindow(window_handle_t window) {}