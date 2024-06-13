#include "cute/shell/CuteShell.hpp"

#include <algorithm>
#include <cstdlib>
#include <locale>
#include <optional>

#include <sdbus-c++/sdbus-c++.h>

namespace fs = std::filesystem;

using namespace cute::shell;

static bool isValidPathVariable(std::string_view value) {
    return !value.empty() && std::ranges::any_of(value, [](char c) { return !std::isblank(c); });
}

static std::optional<fs::path> getPathFromEnvironment(const char *variable, const fs::path &subDirectory = {}) {
    const char *value = std::getenv(variable);

    if (!(value && isValidPathVariable(value)))
        return {};

    fs::path path = value;

    if (!fs::is_directory(path))
        return {};

    return path / subDirectory;
}

// TODO: add getpwuid_r fallback, more validation
fs::path CuteShell::getAppDataPath() {
    return getPathFromEnvironment("XDG_CONFIG_HOME")
            .value_or(getPathFromEnvironment("HOME", ".config").value_or(fs::current_path()));
}

static void revealFile(const fs::path &path) {
    auto connection = sdbus::createSessionBusConnection();

    auto proxy = sdbus::createProxy(
            std::move(connection),
            "org.freedesktop.FileManager1", // destination
            "/org/freedesktop/FileManager1" // object
    );

    proxy->callMethod("ShowItems")
            .onInterface("org.freedesktop.FileManager1")
            .withArguments(std::vector{"file://" + path.string()}, "")
            .dontExpectReply();
}

// TODO: proper error handling
void CuteShell::revealFile(const fs::path &path) {
    try {
        ::revealFile(path);
    }
    catch (const sdbus::Error &) {}
}