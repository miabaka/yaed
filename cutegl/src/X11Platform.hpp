//
// Created by null on 6/3/22.
//

#pragma once

#include "IPlatform.hpp"

extern "C" namespace X11 {
#include "X11/Xlib.h"
}

namespace CuteGL::X11 {
    class X11Platform : public IPlatform {
        ::X11::Display *dpy;

    public:
        X11Platform();
        ~X11Platform() override;

        std::unique_ptr<IWindow> createWindow() override;
    };
}