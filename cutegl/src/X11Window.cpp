//
// Created by null on 6/3/22.
//

#include "X11Window.hpp"

#include <locale>
#include <codecvt>
#include <unordered_map>
#include <set>
#include <string>
#include <cstring>

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
    ::GLX::glXMakeContextCurrent(dpy, wnd.glx, wnd.glx, glx);

    ::GL3W::gl3wInit2((::GL3W::GL3WGetProcAddressProc) ::GLX::glXGetProcAddress);
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
            ::GLX::glXGetProcAddress((const ::GL3W::GLubyte *) ("glXSwapIntervalEXT"));

    globalData.glXCreateContextAttrib = (::GLX::PFNGLXCREATECONTEXTATTRIBSARBPROC)
            ::GLX::glXGetProcAddress((const ::GL3W::GLubyte *) ("glXCreateContextAttribsARB"));

    globalData.initialized = true;
}

X11Window::X11Window(::X11::Display *dpy) : dpy(dpy), _shouldClose(false), _swapInterval(0) {
    if (!globalData.initialized) X11Window::initGlobals(dpy);

    std::printf("X11Window:59\n");

    static constexpr const int defaultX = 100, defaultY = 100, defaultW = 1024, defaultH = 768;

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

    std::printf("X11Window::86: %lx %lx %lx\n", (size_t) visual, (size_t) fbCfg, colormap);

    eventMask = ButtonPressMask | ButtonReleaseMask |
            KeyPressMask | KeyReleaseMask |
            VisibilityChangeMask | ExposureMask;

    ::X11::XSetWindowAttributes attrs = {
        .event_mask = eventMask,
        .colormap = colormap
    };

    wnd = ::X11::XCreateWindow(
            dpy, root,
            defaultX, defaultY, defaultW, defaultH, 0,
            CopyFromParent, InputOutput,
            visual->visual,
            CWColormap | CWEventMask, &attrs);

    ::X11::XFlush(dpy);

    std::printf("X11Window::104: %lx\n", wnd);

    glx = ::GLX::glXCreateWindow(dpy, fbCfg, wnd, nullptr);

    std::printf("X11Window::108: %lx\n", glx);
}

X11Window::~X11Window() {
    ::GLX::glXDestroyWindow(dpy, glx);
    ::X11::XDestroyWindow(dpy, wnd);
}

void X11Window::setSize(glm::uvec2 size) {
    ::X11::XResizeWindow(dpy, wnd, size.x, size.y);
    ::X11::XFlush(dpy);
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
    ::X11::XFlush(dpy);
}

IWindow::DecorationMode X11Window::getDecorations() const {
    return DecorationMode::Default;
}

void X11Window::setTitle(std::wstring_view title) {
    ConvertUtf8 cvt;

    ::X11::XStoreName(dpy, wnd, cvt.to_bytes(title.data()).c_str());
    ::X11::XFlush(dpy);
}

void X11Window::setVisible(bool visible) {
    if (visible) ::X11::XMapWindow(dpy, wnd);
    else ::X11::XUnmapWindow(dpy, wnd);
    ::X11::XFlush(dpy);
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

void X11Window::handleEvents() {
    for (bool &key : keys) key = false;

    ::X11::XSelectInput(dpy, wnd, eventMask);

    ::X11::XEvent evt;

#define K(s, k) { #s, {Key::k} }
#define K2(s, t) { #s, {Key::t} }, { #t, {Key::t} }

    // only those which can be matched by string only
    std::unordered_map<std::string, std::set<Key>> key_strings {
            { " ", {Key::Space} },
            K(\t, Tab), K(\n, Enter), K(`, Grave), K(~, Tilde), K(@, At),
            { "\"", {Key::DoubleQuote} },
            K(#, Hash), K(№, Numero), K($, Dollar), K(;, Semicolon), K(%, Percent),
            K(^, Circumflex), K(:, Colon), K(&, Ampersand), K(?, Question), K(*, Asterisk),
            { "(", {Key::ParenLeft} }, { ")", {Key::ParenRight} },
            K(-, Hyphen), K(_, LowHyphen), K(+, Plus), K(=, Equal), K([, SquareLeft),
            K(], SquareRight), K({, CurlyLeft), K(}, CurlyRight), K(|, VertLine),
            { "\\", {Key::ReverseSlash} }, { "'", {Key::SingleQuote} },
            K(<, Less), K(>, More), { ",", {Key::Comma} }, K(., Dot),
            K2(q, Q), K2(w, W), K2(e, E), K2(r, R), K2(t, T), K2(y, Y), K2(u, U), K2(i, I), K2(o, O), K2(p, P),
            K2(a, A), K2(s, S), K2(d, D), K2(f, F), K2(g, G), K2(h, H), K2(j, J), K2(k, K), K2(l, L),
            K2(z, Z), K2(x, X), K2(c, C), K2(v, V), K2(b, B), K2(n, N), K2(m, M),
            K(0, Kb0), K(1, Kb1), K(2, Kb2), K(3, Kb3), K(4, Kb4),
            K(5, Kb5), K(6, Kb6), K(7, Kb7), K(8, Kb8), K(9, Kb9)
    };

    while (::X11::XPending(dpy) && ::X11::XCheckWindowEvent(dpy, wnd, eventMask, &evt)) {
        switch (evt.type) {
            case KeyPress: {
                ::X11::XKeyEvent &ke = *reinterpret_cast<::X11::XKeyEvent *>(&evt);

                char buffer[255];
                ::X11::XLookupString(reinterpret_cast<::X11::XKeyEvent *>(&evt), buffer, sizeof(buffer), nullptr, nullptr);
                std::string data(buffer);

                if (key_strings.find(data) != key_strings.end()) {
                    for (Key k : key_strings[data]) keys[(size_t) k] = true;
                }

                break;
            }
        }
    }
}

bool X11Window::isKeyPressed(Key key) {
    return keys[(size_t) key];
}
