//
// Created by null on 6/3/22.
//

#pragma once

#include <memory>

#include "IWindow.hpp"

namespace CuteGL {
    struct IPlatform {
        virtual ~IPlatform() = default;

        static std::unique_ptr<IPlatform> create();

        virtual std::unique_ptr<IWindow> createWindow() = 0;
    };
}