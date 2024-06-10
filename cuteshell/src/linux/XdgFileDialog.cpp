#include "XdgFileDialog.hpp"

using namespace cute::shell;

namespace fs = std::filesystem;

static std::string makeGlobPatternCaseInsensitive(std::string_view pattern);

static fs::path makePathFromUri(std::string_view uri);

XdgFileDialog::XdgFileDialog(Type type)
		: _type(type) {}

void XdgFileDialog::addFilter(const Filter &filter) {
	RawFilterPattern pattern = {0, makeGlobPatternCaseInsensitive(filter.mask)};

	RawFilter rawFilter = {std::string(filter.displayName), {std::move(pattern)}};

	_filters.emplace_back(std::move(rawFilter));
	_filterNames.emplace_back(filter.name);
}

void XdgFileDialog::clearFilters() {
	_filters.clear();
	_filterNames.clear();
}

// TODO: handle potential errors
IFileDialog::ShowResult XdgFileDialog::show() {
	if (!(_type == Type::Open || _type == Type::Save))
		return {};

	auto connection = sdbus::createSessionBusConnection();

	auto proxy = sdbus::createProxy(
			*connection,
			"org.freedesktop.portal.Desktop", // destination
			"/org/freedesktop/portal/desktop" // object
	);

	auto methodCall = proxy->createMethodCall(
			"org.freedesktop.portal.FileChooser", // interface
			_type == Type::Save ? "SaveFile" : "OpenFile"
	);

	methodCall << "" << "" << std::map<std::string, sdbus::Variant>{
			{"filters", _filters}
	};

	sdbus::ObjectPath requestObjectPath;
	proxy->callMethod(methodCall) >> requestObjectPath;

	auto requestProxy = sdbus::createProxy(
			*connection,
			"org.freedesktop.portal.Desktop",
			requestObjectPath
	);

	ShowResult result;

	requestProxy->uponSignal("Response")
			.onInterface("org.freedesktop.portal.Request")
			.call(
					[&](uint32_t response, const std::map<std::string, sdbus::Variant> &results) {
						for (auto &uri: results.at("uris").get<std::vector<std::string>>())
							result.paths.emplace_back(makePathFromUri(uri));

						auto currentFilter = results.at("current_filter").get<RawFilter>();
						auto itExistingFilter = std::ranges::find(_filters, currentFilter);

						if (itExistingFilter != _filters.end())
							result.filterName = _filterNames[itExistingFilter - _filters.begin()];

						connection->leaveEventLoop();
					}
			);

	requestProxy->finishRegistration();

	connection->enterEventLoop();

	return result;
}

void XdgFileDialog::setParentWindow(window_handle_t window) {

}

static std::string makeGlobPatternCaseInsensitive(std::string_view pattern) {
	std::string result;

	for (char c: pattern) {
		if (std::isalpha(c)) {
			result.push_back('[');
			result.push_back(static_cast<char>(std::toupper(c)));
			result.push_back(static_cast<char>(std::tolower(c)));
			result.push_back(']');
		} else {
			result.push_back(c);
		}
	}

	return result;
}

static std::optional<int> parseHexDigit(char digit) {
	if (digit >= '0' && digit <= '9')
		return digit - '0';

	if (digit >= 'A' && digit <= 'F')
		return digit - 'A' + 0xa;

	if (digit >= 'a' && digit <= 'f')
		return digit - 'a' + 0xa;

	return {};
}

static std::optional<char> parseEscapedChar(std::string_view string) {
	if (string.length() < 3)
		return {};

	std::optional<int> high = parseHexDigit(string[1]);
	std::optional<int> low = parseHexDigit(string[2]);

	if (!(high && low))
		return {};

	return *high << 4 | *low;
}

static fs::path makePathFromUri(std::string_view uri) {
	const std::string_view schemePrefix = "file://";

	if (uri.starts_with(schemePrefix))
		uri.remove_prefix(schemePrefix.length());

	std::string unescaped;

	while (!uri.empty()) {
		char currentChar = uri[0];

		if (currentChar != '%') {
			unescaped.push_back(currentChar);
			uri.remove_prefix(1);
			continue;
		}

		std::string_view escapedChar = uri.substr(0, 3);
		std::optional<char> maybeActualChar;

		if ((maybeActualChar = parseEscapedChar(escapedChar)))
			unescaped.push_back(*maybeActualChar);
		else
			unescaped.append(escapedChar);

		uri.remove_prefix(escapedChar.length());
	}

	return unescaped;
}
