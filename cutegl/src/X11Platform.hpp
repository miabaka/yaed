//
// Created by null on 6/3/22.
//

#pragma once

#include "cutegl/IPlatform.hpp"

namespace X11 {
    extern "C" {
#include "X11/Xlib.h"
#include "X11/Xutil.h"
    }

    struct MotifWMHints {
        enum class Flags : unsigned long {
            Functions = 1, Decorations = 2
        } flags;

        enum class Functions : unsigned long {
            Nothing  = 0,
            Resize   = 2,
            Move     = 4,
            Minimize = 8,
            Maximize = 16,
            Close    = 32
        } functions;

        enum class Decorations : unsigned long {
            Nothing   = 0,
            Border    = 2,
            Resize    = 4,
            Title     = 8,
            Menu      = 16,
            Minimize  = 32,
            Maximize  = 64
        } decorations;

        long inputMode; // fixme: maybe also a bitset?

        unsigned long state; // fixme: wut is this field?
    };

#define OR(t, base) \
    inline t operator|(t a, t b) { return static_cast<t>(static_cast<base>(a) | static_cast<base>(b)); } \
    inline t operator|=(t &a, t b) { return a = a | b; }
    OR(MotifWMHints::Flags, unsigned long)
    OR(MotifWMHints::Functions, unsigned long)
    OR(MotifWMHints::Decorations, unsigned long)
#undef OR
}

namespace GLX {
    extern "C" {
        using namespace ::X11;
        using namespace ::GL3W;
#include "GL/glx.h"
#include <GL/glxext.h>
    }
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