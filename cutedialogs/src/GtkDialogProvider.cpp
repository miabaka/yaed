//
// Created by null on 6/5/22.
//

#include "GtkDialogProvider.hpp"
#include "GtkFileDialog.hpp"

using namespace cute::dialogs;

std::unique_ptr<IFileDialog> GtkDialogProvider::createFileDialog(IFileDialog::Type type) {
    return std::make_unique<GtkFileDialog>(type);
}

GtkDialogProvider::GtkDialogProvider() {
    int argc = 0;
    gtk_init(&argc, nullptr);
    bindtextdomain("cutedialogs", "/usr/share/locale"); // fixme: don't hardcode this
    bind_textdomain_codeset("cutedialogs", "UTF-8");
    textdomain("cutedialogs");
}
