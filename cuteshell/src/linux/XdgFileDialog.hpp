#include "cute/shell/IFileDialog.hpp"

#include <string>
#include <vector>

#include <sdbus-c++/sdbus-c++.h>

namespace cute::shell {

class XdgFileDialog : public IFileDialog {
public:
	explicit XdgFileDialog(Type type);

	void addFilter(const Filter &filter) override;

	void clearFilters() override;

	ShowResult show() override;

	void setParentWindow(window_handle_t window) override;

private:
	using RawFilterPattern = sdbus::Struct<unsigned int, std::string>;
	using RawFilter = sdbus::Struct<std::string, std::vector<RawFilterPattern>>;

	enum class PortalResponseCode {
		Success = 0,
		Cancelled = 1,
		Other = 2
	};

	using PortalResponseData = const std::map<std::string, sdbus::Variant>;

	std::vector<RawFilter> _filters;
	std::vector<std::string> _filterNames;
	const Type _type;

	void readPortalResponse(PortalResponseCode code, const PortalResponseData &data, ShowResult &out);
};

}