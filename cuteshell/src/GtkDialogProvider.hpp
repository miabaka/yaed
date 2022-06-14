//
// Created by null on 6/5/22.
//

#pragma once

#include "cute/shell/IDialogProvider.hpp"

namespace cute::shell {

class GtkDialogProvider : public IDialogProvider {
public:
    GtkDialogProvider();

    std::unique_ptr<IFileDialog> createFileDialog(IFileDialog::Type type) override;
};

}