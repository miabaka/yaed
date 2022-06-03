//
// Created by null on 6/3/22.
//

#pragma once

#include "IWindow.hpp"
#include "X11Platform.hpp" // for Xlib

namespace CuteGL::X11 {
    class X11Window;

    class X11Context : public IWindowContext {
    public:
        explicit X11Context(X11Window const &wnd);

        ~X11Context() override;

        void makeCurrent() override;
    };

    class X11Window : public IWindow {
        ::X11::Window wnd;
        ::GLX::GLXWindow glx;
        ::X11::Display *dpy;

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
    };
}