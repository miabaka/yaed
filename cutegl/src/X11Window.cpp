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

X11Window::X11Window() {

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
