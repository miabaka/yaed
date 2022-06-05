#pragma once

#include <filesystem>
#include <vector>

namespace cute::dialogs {

class IFileDialog {
public:
	enum class Type {
		Open,
		Save,
		PeekFolders
	};

	virtual ~IFileDialog() = default;

	virtual void addExtensionFilter(const std::string &name, const std::string &extension) = 0;

	virtual void addExtensionFilter(const std::wstring &name, const std::string &extension) = 0;

	virtual void clearExtensionFilters() = 0;

	virtual std::filesystem::path show() = 0;

	virtual std::vector<std::filesystem::path> showForMultiple() = 0;
};

} // namespace cute::dialogs