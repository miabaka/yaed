//
// Created by null on 6/3/22.
//

#pragma once

#include <memory>

#include "IWindow.hpp"
#include "Key.hpp"

namespace GL3W {
#include <GL/gl3w.h>
}

namespace CuteGL {
    struct IPlatform {
        virtual ~IPlatform() = default;

        static std::unique_ptr<IPlatform> create();

        virtual std::unique_ptr<IWindow> createWindow() = 0;
    };
}