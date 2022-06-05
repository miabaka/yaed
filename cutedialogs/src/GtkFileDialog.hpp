//
// Created by null on 6/5/22.
//

#pragma once

#include "cute/dialogs/IFileDialog.hpp"

#include <gtk-3.0//gtk/gtk.h>

namespace cute::dialogs {

class GtkFileDialog : public IFileDialog {
    GtkWidget *dialog;

    std::filesystem::path getPath(int result);
public:
    GtkFileDialog(Type type);

    ~GtkFileDialog() override = default;

    void addExtensionFilter(const std::string &name, const std::string &extension) override;

    void addExtensionFilter(const std::wstring &name, const std::string &extension) override;

    void clearExtensionFilters() override;

    std::filesystem::path show() override;

    std::filesystem::path showForMultiple() override;
};

}