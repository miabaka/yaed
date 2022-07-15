//
// Created by null on 6/5/22.
//

#include <iostream>

#include <cutegl/IPlatform.hpp>

#include <glm/vec3.hpp>

using namespace CuteGL;

int main() {
    std::unique_ptr<IPlatform> platform = IPlatform::create();

    std::unique_ptr<IWindow> window = platform->createWindow();

    window->setTitle(L"Hello CuteGL!");
    window->setSize({ 1024, 768 });

    window->setSwapInterval(1);

    std::unique_ptr<IWindowContext> ctx = window->createContext();

    window->setVisible(true);

    std::cout << "after window->setVisible" << std::endl;

    glm::vec3 clearColor{ 0.f, 0.f, 0.f };

    while (!window->shouldClose()) {
        window->handleEvents();

        if (window->isKeyPressed(Key::X)) {
            std::cout << "Baka!" << std::endl;
            clearColor.x += 0.1f;
        }

        ::GL3W::glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.f);
        ::GL3W::glClear(GL_COLOR_BUFFER_BIT);

        window->swapBuffers();
    }

    return 0;
}