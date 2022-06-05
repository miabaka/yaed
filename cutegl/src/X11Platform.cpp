//
// Created by null on 6/3/22.
//

#include "X11Platform.hpp"
#include "X11Window.hpp"

#include <cstdio>

using namespace CuteGL::X11;
using namespace CuteGL;

X11Platform::X11Platform() {
    dpy = ::X11::XOpenDisplay(nullptr);

    ::X11::XSetErrorHandler([](::X11::Display *d, ::X11::XErrorEvent *e) -> int {
        std::fprintf(stderr, "X11 error: %x %x %lx\n", e->error_code, e->minor_code, e->resourceid);
        return 0;
    });
}

X11Platform::~X11Platform() {
    ::X11::XCloseDisplay(dpy);
}

std::unique_ptr<IWindow> X11Platform::createWindow() {
    return std::make_unique<X11Window>(dpy);
}

