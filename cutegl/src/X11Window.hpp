//
// Created by null on 6/3/22.
//

#pragma once

#include "cutegl/IWindow.hpp"
#include "X11Platform.hpp" // for Xlib

namespace CuteGL::X11 {
    class X11Window;

    class X11Context : public IWindowContext {
        ::X11::Display *dpy;
        ::GLX::GLXContext glx;
        ::GLX::GLXWindow wnd;

    public:
        explicit X11Context(X11Window const &wnd);

        ~X11Context() override;

        void makeCurrent() override;
    };

    class X11Window : public IWindow {
        ::X11::Window wnd;
        ::GLX::GLXWindow glx;
        ::X11::Display *dpy;

        ::GLX::GLXFBConfig fbCfg;

        static void initGlobals(::X11::Display *dpy);

        bool _shouldClose;
        int _swapInterval;

        bool keys[(std::size_t) CuteGL::Key::MaxValue] {false};

        long eventMask;

        friend class X11Context; // fixme: don't think this is good practice; don't kill me pls >.<
    public:
        X11Window(::X11::Display *dpy);

        ~X11Window() override;

        void setSize(glm::uvec2 size) override;

        void setDecorations(DecorationMode mode) override;

        DecorationMode getDecorations() const override;

        void setTitle(std::wstring_view title) override;

        void setVisible(bool visible) override;

        bool shouldClose() const override;

        void setShouldClose(bool shouldClose) override;

        int getSwapInterval() const override;

        void setSwapInterval(int interval) override;

        std::unique_ptr<IWindowContext> createContext() override;

        void swapBuffers() override;

        void handleEvents() override;

        bool isKeyPressed(Key key) override;
    };
}