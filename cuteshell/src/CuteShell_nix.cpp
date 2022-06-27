#include "cute/shell/CuteShell.hpp"

#include <unordered_map>
#include <sstream>
#include <functional>
#include <iterator>

#include <dbus/dbus.h>
#include <fmt/format.h>

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
static Container getUntil(Iter &it, Iter end, const std::function<bool(char)> &predicate) {
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

void CuteShell::revealFile(const fs::path &path) {
    using fmt::format;

    static DBusConnection *conn = nullptr;

    DBusError err;
    dbus_error_init(&err);

#define FU "DBus says: fak u, "
#define FUE FU "here's the error you definitely won't need btw: "
#define FUEF FUE "'{}'"
#define BAKA FU "u r baka "

    if (!conn) {
        conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
        if (dbus_error_is_set(&err)) {
            std::string message = err.message;
            dbus_error_free(&err);
            throw std::runtime_error(format(FUEF, message));
        }

        if (!conn)
            throw std::runtime_error(BAKA "and won't get a connection!");

        int ret = dbus_bus_request_name(conn, "moe.miabaka.yaed.baka", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
        if (dbus_error_is_set(&err)) {
            std::string message = err.message;
            dbus_error_free(&err);
            throw std::runtime_error(
                    format(FUEF, message));
        }

        if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
            throw std::runtime_error(FU "looks like you have two instances running; don't do that! *angry emoji*");
    }

    DBusMessage *msg;
    DBusMessageIter args, array;
    DBusPendingCall *pending;

    msg = dbus_message_new_method_call("org.freedesktop.FileManager1", "/org/freedesktop/FileManager1", "org.freedesktop.FileManager1", "ShowItems");
    if (!msg)
        throw std::runtime_error(BAKA "and won't get any messages! *angry*");

    std::string url = fmt::format("file://{}", absolute(path).c_str());
    char const *url_cstr = url.c_str();
    char const *empty_string = "";

#define SSEH FU "some sorta error happened "
#define NMWCDH "... *shrug* not much we can do here, ig"

    dbus_message_iter_init_append(msg, &args);

    if (!dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "s", &array)) {
        throw std::runtime_error(SSEH "while creating an argument array" NMWCDH);
    }

    if (!dbus_message_iter_append_basic(&array, DBUS_TYPE_STRING, &url_cstr)) {
        throw std::runtime_error(SSEH "while adding file url to array" NMWCDH);
    }

    if (!dbus_message_iter_close_container(&args, &array)) {
        throw std::runtime_error(SSEH "while finishing work with that array" NMWCDH);
    }

    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &empty_string)) {
        throw std::runtime_error(SSEH "while adding some sort of additional param we don't know what is for" NMWCDH);
    }

    if (!dbus_connection_send_with_reply(conn, msg, &pending, -1)) {
        throw std::runtime_error(SSEH "while sending message" NMWCDH);
    }

    if (!pending) {
        throw std::runtime_error(SSEH "and there's no pending reply" NMWCDH);
    }

    dbus_connection_flush(conn);

    dbus_message_unref(msg);

    dbus_pending_call_block(pending);

    msg = dbus_pending_call_steal_reply(pending);
    if (!msg) {
        throw std::runtime_error(SSEH "-- there really is no reply" NMWCDH);
    }

    dbus_pending_call_unref(pending);
    dbus_message_unref(msg);
}