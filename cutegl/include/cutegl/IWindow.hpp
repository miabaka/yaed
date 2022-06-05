//
// Created by null on 6/3/22.
//

#pragma once

#include <string_view>
#include <memory>
#include "glm/vec2.hpp"

#include "Key.hpp"

namespace CuteGL {
    /**
     * RAII-enabled context.
     */
    struct IWindowContext {
        virtual ~IWindowContext() = default;

        virtual void makeCurrent() = 0;
    };

    struct IWindow {
        enum class DecorationMode {
            Default,
            NoCaption,
            Undecorated
        };

        virtual ~IWindow() = default;

        virtual void setSize(glm::uvec2 size) = 0;

        virtual void setDecorations(DecorationMode mode) = 0;
        virtual DecorationMode getDecorations() const = 0;

        virtual void setTitle(std::wstring_view title) = 0;

        virtual void setVisible(bool visible) = 0;

        virtual bool shouldClose() const = 0;
        virtual void setShouldClose(bool shouldClose) = 0;

        virtual int getSwapInterval() const = 0;
        virtual void setSwapInterval(int interval) = 0;

        virtual std::unique_ptr<IWindowContext> createContext() = 0;

        virtual void swapBuffers() = 0;

        virtual void handleEvents() = 0;
        virtual bool isKeyPressed(Key key) = 0;
    };
}