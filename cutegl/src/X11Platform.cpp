//
// Created by null on 6/3/22.
//

#include "X11Platform.hpp"
#include "X11Window.hpp"

using namespace CuteGL::X11;
using namespace CuteGL;

X11Platform::X11Platform() {
    dpy = ::X11::XOpenDisplay(nullptr);
}

X11Platform::~X11Platform() {
    ::X11::XCloseDisplay(dpy);
}

std::unique_ptr<IWindow> X11Platform::createWindow() {
    return std::unique_ptr<X11Window>();
}

