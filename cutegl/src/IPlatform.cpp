//
// Created by null on 6/3/22.
//

#include "IPlatform.hpp"
#include "X11Platform.hpp"

using namespace CuteGL;

std::unique_ptr<IPlatform> IPlatform::create() {
    return std::make_unique<X11::X11Platform>();
}
