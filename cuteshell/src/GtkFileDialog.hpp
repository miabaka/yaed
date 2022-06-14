//
// Created by null on 6/5/22.
//

#pragma once

#include <gtk/gtk.h>
#include "cute/shell/IFileDialog.hpp"

namespace cute::shell {

class GtkFileDialog : public IFileDialog {
public:
    explicit GtkFileDialog(Type type);

    ~GtkFileDialog() override = default;

    void addExtensionFilter(const std::string &name, const std::string &extension) override;

    void addExtensionFilter(const std::wstring &name, const std::string &extension) override;

    void clearExtensionFilters() override;

    std::filesystem::path show() override;

    std::vector<std::filesystem::path> showForMultiple() override;

private:
    GtkWidget *_dialog;

    std::filesystem::path getPath(int result);
};

}