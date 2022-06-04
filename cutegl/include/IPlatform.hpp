//
// Created by null on 6/3/22.
//

#pragma once

#include <memory>

#include "IWindow.hpp"

namespace CuteGL {
    enum class Key {
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        Ins, Del, PrtSc, Home, End, PgUp, PgDn,
        NumLk, CapsLk,
        Tab, BkSp, Enter,
        LShift, RShift, LCtrl, RCtrl, LAlt, RAlt,
        Space,
        Grave, Tilde, Bang, At, DoubleQuote, Hash, Numero, Dollar, Semicolon, Circumflex, Colon, Ampersand, Question,
        Asterisk, ParenLeft, ParenRi, Hyphen, LowHyphen, Plus, Equal, SquareLeft, SquareRight, CurlyLeft, CurlyRight,
        VertLine, ReverseSlash, Slash, SingleQuote, Less, More, Comma, Dot,
        Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M,
        NumpadSlash, NumpadAsterisk, NumpadMinus, NumpadPlus, NumpadEnter, NumpadDot,
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        Up, Down, Left, Right
    };

    struct IPlatform {
        virtual ~IPlatform() = default;

        static std::unique_ptr<IPlatform> create();

        virtual std::unique_ptr<IWindow> createWindow() = 0;

        virtual bool isKeyPressed(Key key) = 0;
    };
}