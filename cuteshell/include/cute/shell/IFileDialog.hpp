#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

#include "CuteShell.hpp"

namespace cute::shell {

class IFileDialog {
public:
    enum class Type {
        Open,
        Save,
        PeekFolders
    };

    struct Filter {
        std::string_view name;
        std::string_view displayName;
        std::string_view mask;
    };

	struct ShowResult {
		std::string filterName;
		std::vector<std::filesystem::path> paths;
	};

    virtual ~IFileDialog() = default;

    virtual void addFilter(const Filter &filter) = 0;

    virtual void clearFilters() = 0;

    virtual ShowResult show() = 0;

    virtual void setParentWindow(window_handle_t window) = 0;
};

} // namespace cute::dialogs