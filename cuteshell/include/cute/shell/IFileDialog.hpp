#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

namespace cute::shell {

class IFileDialog {
public:
	enum class Type {
		Open,
		Save,
		PeekFolders
	};

	virtual ~IFileDialog() = default;

	virtual void addExtensionFilter(std::string_view name, std::string_view extension) = 0;

	virtual void clearExtensionFilters() = 0;

	virtual std::filesystem::path show() = 0;

	virtual std::vector<std::filesystem::path> showForMultiple() = 0;
};

} // namespace cute::dialogs