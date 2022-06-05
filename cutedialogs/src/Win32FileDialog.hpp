#pragma once

#include <list>
#include <cute/dialogs/IFileDialog.hpp>

namespace cute::dialogs {

class Win32FileDialog : public IFileDialog {
public:
	Win32FileDialog(Type type);

	~Win32FileDialog() override;

	void addExtensionFilter(const std::string &name, const std::string &extension) override;

	void addExtensionFilter(const std::wstring &name, const std::string &extension) override;

	void clearExtensionFilters() override;

	std::filesystem::path show() override;

	std::vector<std::filesystem::path> showForMultiple() override;

private:
	struct Filter {
		std::wstring name;
		std::wstring extension;
	};

	Type _type;
	void *_handle;
	std::list<Filter> _extensionFilters;
};

} // namespace cute::dialogs