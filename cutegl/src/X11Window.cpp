//
// Created by null on 6/3/22.
//

#include "X11Window.hpp"

using namespace CuteGL::X11;
using namespace CuteGL;

X11Context::X11Context(const X11Window &wnd) {

}

X11Context::~X11Context() {

}

void X11Context::makeCurrent() {

}

X11Window::X11Window(::X11::Display *dpy) : dpy(dpy) {
    static constexpr const int defaultX = 0, defaultY = 0, defaultW = 1024, defaultH = 768;

    static constexpr const int fbCfgAttribs[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DOUBLEBUFFER, True,
        None
    };

    using _XPrivDisplay = ::X11::_XPrivDisplay; // for macros to work
    auto screen = DefaultScreen(dpy);
    auto root = RootWindow(dpy, screen);

    int fbCfgCount;
    auto fbCfg = ::GLX::glXChooseFBConfig(dpy, screen, fbCfgAttribs, &fbCfgCount);
    auto visual = ::GLX::glXGetVisualFromFBConfig(dpy, *fbCfg);

    auto colormap = ::X11::XCreateColormap(dpy, root, visual, AllocNone); // fixme: i guess AllocNone should be fine, but who knows?

    ::X11::XSetWindowAttributes attrs = {
        .colormap = colormap,
        .event_mask = ButtonPressMask | ButtonReleaseMask |
                KeyPressMask | KeyReleaseMask |
                VisibilityChangeMask | ExposureMask
    };

    wnd = ::X11::XCreateWindow(
            dpy, root,
            defaultX, defaultY, defaultW, defaultH, 0,
            CopyFromParent, InputOutput,
            visual,
            CWColormap | CWEventMask, &attrs);

    glx = ::GLX::glXCreateWindow(dpy, fbCfg, wnd, nullptr);
}

X11Window::~X11Window() {
    ::GLX::glXDestroyWindow(dpy, glx);
    ::X11::XDestroyWindow(dpy, wnd);
}

void X11Window::setSize(glm::uvec2 size) {

}

void X11Window::setDecorations(IWindow::DecorationMode mode) {

}

IWindow::DecorationMode X11Window::getDecorations() const {
    return DecorationMode::Default;
}

void X11Window::setTitle(std::wstring_view title) {

}

void X11Window::setVisible(bool visible) {

}

bool X11Window::shouldClose() const {
    return false;
}

void X11Window::setShouldClose(bool shouldClose) {

}

int X11Window::getSwapInterval() const {
    return 0;
}

void X11Window::setSwapInterval(int interval) {

}

std::unique_ptr<IWindowContext> X11Window::createContext() {
    return std::make_unique<X11Context>(*this);
}

void X11Window::swapBuffers() {

}
