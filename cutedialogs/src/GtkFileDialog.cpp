//
// Created by null on 6/5/22.
//

#include "GtkFileDialog.hpp"

#include <unordered_map>

#include <glibmm-2.4/glibmm/i18n.h>

using namespace cute::dialogs;

GtkFileDialog::GtkFileDialog(IFileDialog::Type type) {
    static std::unordered_map<IFileDialog::Type, const char *> names {
            { IFileDialog::Type::Open, "Open File" },
            { IFileDialog::Type::Save, "Save File" },
            { IFileDialog::Type::PeekFolders, "Open Folder" }
    };

    static std::unordered_map<IFileDialog::Type, GtkFileChooserAction> actions {
            { IFileDialog::Type::Open, GTK_FILE_CHOOSER_ACTION_OPEN },
            { IFileDialog::Type::Save, GTK_FILE_CHOOSER_ACTION_SAVE },
            { IFileDialog::Type::PeekFolders, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER }
    };

    const char *accept_btn = type == IFileDialog::Type::Save ? "_Save" : "_Open";

    dialog = gtk_file_chooser_dialog_new(
            names[type],
            nullptr,
            actions[type],
            _("_Cancel"), GTK_RESPONSE_CANCEL, _(accept_btn), GTK_RESPONSE_ACCEPT, nullptr);
}

void GtkFileDialog::addExtensionFilter(const std::wstring &name, const std::string &extension) {
}

void GtkFileDialog::addExtensionFilter(const std::string &name, const std::string &extension) {
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, name.c_str());
    gtk_file_filter_add_pattern(filter, extension.c_str());
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
}

void GtkFileDialog::clearExtensionFilters() {
//    GListModel *filters = gtk_file_chooser_get_filters(GTK_FILE_CHOOSER(dialog));
    GSList *filters = gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(dialog));
    guint count = g_slist_length(filters);

    GtkFileFilter *_filters[count];
    for (guint i = 0; i < count; i++) _filters[i] = GTK_FILE_FILTER(g_slist_nth(filters, i));

    for (int i = 0; i < count; i++) {
        gtk_file_chooser_remove_filter(GTK_FILE_CHOOSER(dialog), _filters[i]);
    }
}

std::filesystem::path GtkFileDialog::getPath(int result) {
    if (result == GTK_RESPONSE_ACCEPT) {
        GFile *f = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));

        char *path_cstr = g_file_get_path(f);
        std::string path(path_cstr);
        g_free(path_cstr);

        return {path};
    }

    return {};
}

std::filesystem::path GtkFileDialog::show() {
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));

    std::filesystem::path path = getPath(res);

    gtk_widget_destroy(dialog);
    while (gtk_events_pending()) gtk_main_iteration();

    return std::move(path);
}

std::filesystem::path GtkFileDialog::showForMultiple() {
    return {};
}
