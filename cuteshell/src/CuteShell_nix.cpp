#include "cute/shell/CuteShell.hpp"

#include <unordered_map>
#include <sstream>
#include <functional>
#include <iterator>

using namespace cute::shell;
namespace fs = std::filesystem;

static const std::unordered_map<std::string, std::string> defaults {
        { "CONFIG", "$HOME/.config" },
        { "DATA", "$HOME/.local/share" },
        { "STATE", "$HOME/.local/state" }
};

static const char *getXdgPath(const std::string &name) {
    std::string full = "XDG_" + name + "_HOME";
    const char *env = std::getenv(full.c_str());

    if (env == nullptr) {
        if (defaults.find(name) == defaults.end())
            return nullptr;
        return defaults.at(name).c_str();
    }

    return env;
}

template<class Container, class Iter = typename Container::const_iterator>
Container getUntil(Iter &it, Iter end, const std::function<bool(char)> &predicate) {
    if (it == end || !predicate(*it)) return {};

    Container str;

    str.resize(std::count_if(it, end, predicate));

    auto cur = it;
    for (int i = 0; predicate(*cur) && cur != end; cur++, i++)
        str[i] = *cur;

    it = cur;
    return str;
}

fs::path CuteShell::getAppDataPath() {
    static const std::string dirName = "CONFIG";
    std::string dirPath(getXdgPath(dirName));

    std::stringstream dir;
    for (auto it = dirPath.cbegin(); it != dirPath.cend() && *it != '\0';) {
        if (*it != '$') { // write characters until env var
            dir << getUntil<std::string>(it, dirPath.cend(), [](char c) {
                return c != '$' && c != '\0';
            });
        } else {
            auto var = getUntil<std::string>(it, dirPath.cend(), [](char c) {
                return c != '/' && c != '\0'; // FIXME: there's probably a better way.
            });

            // skip the $ sign
            char const *val = std::getenv(&var.c_str()[1]);

            if (val != nullptr) dir << val;
        }
    }

    return { dir.str() };
}

void CuteShell::revealFile(const std::filesystem::path &path) {}