//
// Created by null on 6/3/22.
//

#pragma once

#include <memory>

namespace CuteGL {
    struct IPlatform {
        virtual ~IPlatform() = default;

        static std::unique_ptr<IPlatform> create();
    };
}