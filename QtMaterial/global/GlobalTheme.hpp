#pragma once

#include "MaterialTheme.hpp"
#include "init.hpp"

namespace material {

inline global::MaterialTheme& globalTheme() {
    static global::MaterialTheme instance(GlobalSeedColor);
    return instance;
}

} // namespace material