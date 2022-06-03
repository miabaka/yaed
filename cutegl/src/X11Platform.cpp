//
// Created by null on 6/3/22.
//

#include "X11Platform.hpp"

using namespace CuteGL::X11;

X11Platform::X11Platform() {
    dpy = ::X11::XOpenDisplay(nullptr);
}

X11Platform::~X11Platform() {
    ::X11::XCloseDisplay(dpy);
}