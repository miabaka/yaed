//
// Created by null on 6/5/22.
//

#include <iostream>

#include <cutegl/IPlatform.hpp>

using namespace CuteGL;

int main() {
    std::unique_ptr<IPlatform> platform = IPlatform::create();

    std::unique_ptr<IWindow> window = platform->createWindow();

    window->setTitle(L"Hello CuteGL!");
    window->setSize({ 1024, 768 });

    window->setVisible(true);

    std::cout << "after window->setVisible" << std::endl;

    while (!window->shouldClose());

    return 0;
}