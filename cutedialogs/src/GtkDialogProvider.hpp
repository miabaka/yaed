//
// Created by null on 6/5/22.
//

#pragma once

#include "cute/dialogs/IDialogProvider.hpp"

namespace cute::dialogs {

class GtkDialogProvider : public IDialogProvider {
public:
    GtkDialogProvider();

    std::unique_ptr<IFileDialog> createFileDialog(IFileDialog::Type type) override;
};

}