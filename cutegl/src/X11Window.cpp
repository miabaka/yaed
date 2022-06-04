//
// Created by null on 6/3/22.
//

#include "X11Window.hpp"

#include <locale>
#include <codecvt>

using namespace CuteGL::X11;
using namespace CuteGL;

using ConvertUtf8 = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>;

static struct {
    bool initialized;

    ::X11::Atom motifWmHints;
    ::GLX::PFNGLXSWAPINTERVALEXTPROC glXSwapInterval;
    ::GLX::PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttrib;
} globalData = { .initialized = false };

X11Context::X11Context(const X11Window &wnd) : dpy(wnd.dpy), wnd(wnd.glx) {
    static constexpr const int ctxAttribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3, // fixme: maybe pass GL version in params?
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
    };

    glx = globalData.glXCreateContextAttrib(dpy, wnd.fbCfg, nullptr, true, ctxAttribs);
}

X11Context::~X11Context() {
    ::GLX::glXDestroyContext(dpy, glx);
}

void X11Context::makeCurrent() {
    ::GLX::glXMakeContextCurrent(dpy, wnd, wnd, glx);
}

void X11Window::initGlobals(::X11::Display *dpy) {
    globalData.motifWmHints = ::X11::XInternAtom(dpy, "_MOTIF_WM_HINTS", True);

    globalData.glXSwapInterval = (::GLX::PFNGLXSWAPINTERVALEXTPROC)
            ::GLX::glXGetProcAddress((const ::GLX::GLubyte *) ("glXSwapIntervalEXT"));

    globalData.glXCreateContextAttrib = (::GLX::PFNGLXCREATECONTEXTATTRIBSARBPROC)
            ::GLX::glXGetProcAddress((const ::GLX::GLubyte *) ("glXCreateContextAttribsARB"));

    globalData.initialized = true;
}

X11Window::X11Window(::X11::Display *dpy) : dpy(dpy) {
    if (!globalData.initialized) X11Window::initGlobals(dpy);

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
    fbCfg = *::GLX::glXChooseFBConfig(dpy, screen, fbCfgAttribs, &fbCfgCount);
    auto visual = ::GLX::glXGetVisualFromFBConfig(dpy, fbCfg);

    auto colormap = ::X11::XCreateColormap(dpy, root, visual->visual, AllocNone); // fixme: i guess AllocNone should be fine, but who knows?

    ::X11::XSetWindowAttributes attrs = {
        .event_mask = ButtonPressMask | ButtonReleaseMask |
                      KeyPressMask | KeyReleaseMask |
                      VisibilityChangeMask | ExposureMask,
        .colormap = colormap
    };

    wnd = ::X11::XCreateWindow(
            dpy, root,
            defaultX, defaultY, defaultW, defaultH, 0,
            CopyFromParent, InputOutput,
            visual->visual,
            CWColormap | CWEventMask, &attrs);

    glx = ::GLX::glXCreateWindow(dpy, fbCfg, wnd, nullptr);
}

X11Window::~X11Window() {
    ::GLX::glXDestroyWindow(dpy, glx);
    ::X11::XDestroyWindow(dpy, wnd);
}

void X11Window::setSize(glm::uvec2 size) {
    ::X11::XResizeWindow(dpy, wnd, size.x, size.y);
}

void X11Window::setDecorations(IWindow::DecorationMode mode) {
    using Decorations = ::X11::MotifWMHints::Decorations;
    using Functions   = ::X11::MotifWMHints::Functions;
    using Flags       = ::X11::MotifWMHints::Flags;

    static const Functions defaultFns =
            Functions::Close | Functions::Resize | Functions::Maximize |
            Functions::Minimize | Functions::Move;

    ::X11::MotifWMHints hints {
        .flags = Flags::Functions | Flags::Decorations,
        .functions = defaultFns,
        .decorations = Decorations::Nothing,
        .inputMode = 0, .state = 0
    };

    switch (mode) {
        case IWindow::DecorationMode::Default:
            hints.decorations |= Decorations::Resize | Decorations::Minimize | Decorations::Maximize |
                    Decorations::Title | Decorations::Menu;
        case IWindow::DecorationMode::NoCaption:
            hints.decorations |= Decorations::Border;
        case IWindow::DecorationMode::Undecorated:
            break;
    }

    ::X11::XChangeProperty(
            dpy, wnd,
            globalData.motifWmHints, globalData.motifWmHints,
            32,
            PropModeReplace,
            (unsigned char *) &hints, 5);
}

IWindow::DecorationMode X11Window::getDecorations() const {
    return DecorationMode::Default;
}

void X11Window::setTitle(std::wstring_view title) {
    ConvertUtf8 cvt;

    ::X11::XStoreName(dpy, wnd, cvt.to_bytes(title.data()).c_str());
}

void X11Window::setVisible(bool visible) {
    if (visible) ::X11::XMapWindow(dpy, wnd);
    else ::X11::XUnmapWindow(dpy, wnd);
}

bool X11Window::shouldClose() const {
    return _shouldClose;
}

void X11Window::setShouldClose(bool shouldClose) {
    _shouldClose = shouldClose;
}

int X11Window::getSwapInterval() const {
    return _swapInterval;
}

void X11Window::setSwapInterval(int interval) {
    _swapInterval = interval;
    globalData.glXSwapInterval(dpy, glx, interval);
}

std::unique_ptr<IWindowContext> X11Window::createContext() {
    return std::make_unique<X11Context>(*this);
}

void X11Window::swapBuffers() {
    ::GLX::glXSwapBuffers(dpy, glx);
}
