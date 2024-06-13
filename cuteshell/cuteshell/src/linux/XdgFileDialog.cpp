#include "XdgFileDialog.hpp"

#include <utility>

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

	methodCall << _parentWindow << "" << std::map<std::string, sdbus::Variant>{
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
					[&](unsigned int code, const PortalResponseData &data) {
						readPortalResponse(static_cast<PortalResponseCode>(code), data, result);
						connection->leaveEventLoop();
					}
			);

	requestProxy->finishRegistration();

	connection->enterEventLoop();

	return result;
}

void XdgFileDialog::setParentWindow(WindowHandle window) {
	_parentWindow = std::move(window);
}

// TODO: return error in case some of the required data fields don't exist
void XdgFileDialog::readPortalResponse(
		PortalResponseCode code, const PortalResponseData &data, ShowResult &out) {
	using UriList = std::vector<std::string>;

	if (code != PortalResponseCode::Success)
		return;

	auto itUris = data.find("uris");
	auto itCurrentFilter = data.find("current_filter");

	if (itUris != data.end() && itUris->second.containsValueOfType<UriList>()) {
		for (auto &uri: itUris->second.get<UriList>())
			out.paths.emplace_back(makePathFromUri(uri));
	}

	if (itCurrentFilter == data.end() || !itCurrentFilter->second.containsValueOfType<RawFilter>())
		return;

	auto itKnownFilterMatch = std::ranges::find(
			_filters, itCurrentFilter->second.get<RawFilter>());

	if (itKnownFilterMatch == _filters.end())
		return;

	out.filterName = _filterNames[itKnownFilterMatch - _filters.begin()];
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
