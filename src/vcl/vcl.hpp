#pragma once

/*! \mainpage VCL Code Documentation
 *
 * # Introduction
 *
 * VCL - Visual Computing Library - is a simple lightweight library on top of OpenGL provided to ease learning of 3D programming while avoiding re-coding everything from scratch (matrices, mesh structures, etc.). The library provides several helper structure and functions to set up 3D scene with interactive and/or animated elements.
 *
 * The objective of the library is to be simple as simple as possible to read and use. The majority of VCL structures and functions are aimed to be minimalistic without hidden states. The code remains fully compatible with direct raw OpenGL calls and user-defined shaders. The animation loop itself is entirely defined by the user.
 *
 * # Dowload, compile and use
 *
 * - For INF443: \url https://github.com/drohmer/inf443_vcl
 *
 * _Instruction to compile and execute are directly provided in the github page_
 *
 */

#include "base/base.hpp"
#include "math/math.hpp"
#include "opengl/opengl.hpp"
#include "interaction/interaction.hpp"
#include "shape/shape.hpp"
#include "wrapper/wrapper.hpp"
#include "containers/containers.hpp"


