#pragma once

#include <array>

#include "../stl/stl.hpp"

namespace vcl
{
/** Stores 2-index {a,b}. \ingroup container */
using size_t2 = std::array<size_t,2>;

/** Stores 3-index {a,b,c}. \ingroup container */
using size_t3 = std::array<size_t,3>;

/** Stores 3-unsigned int {a,b,c}.
 * use for triangle connectivity in OpenGL. \ingroup container */
using uint3  = std::array<unsigned int, 3>;

/** Stores 3-int {a,b,c}. \ingroup container */
using int3 = std::array<int,3>;
}

