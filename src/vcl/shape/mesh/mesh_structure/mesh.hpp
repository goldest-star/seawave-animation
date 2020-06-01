#pragma once

#ifdef __linux__
#pragma GCC diagnostic ignored "-Weffc++"
#endif 

#include "../../../math/vec/vec.hpp"
#include "vcl/base/base.hpp"
#include "vcl/containers/buffer/buffer.hpp"

#include <vector>
#include <array>

namespace vcl
{



/** A mesh structure stores the per-vertex informations as well as the triangle connectivity.
 *  All buffers are stored contiguously in the CPU memory.
*/
struct mesh
{
    /** Per-vertex attributes */
    buffer<vec3> position;
    buffer<vec3> normal;
    buffer<vec4> color;
    buffer<vec2> texture_uv;

    /** Triangle connectivity */
    buffer<uint3> connectivity;

    /** Fill all per-vertex attributes with default values if they are empty (ex. color to white, and 0 for texture-uv)*/
    void fill_empty_fields();
    /** Add a mesh structure to the current one (concatenate per-vertex attributes, and add triangle indices accordingly)*/
    void push_back(const mesh& mesh_to_add);

    /** Fill per-vertex color value with a constant value */
    void fill_color_uniform(const vec3& c);
    void fill_color_uniform(const vec4& c);

};

/** Compute per-vertex normals given a set of position coordinates and triangle index connectivity */
buffer<vec3> normal(const buffer<vec3>& position, const buffer<uint3>& connectivity);

/** Compute per-vertex normals given a set of position coordinates and triangle index connectivity
    Avoid re-allocation of normal vector if it is not needed */
void normal(const buffer<vec3>& position, const buffer<uint3>& connectivity, buffer<vec3>& normals, bool invert=false);


vec3 center_of_mass(const mesh& shape);
vec3 center_of_mass(const buffer<vec3>& position, const buffer<uint3>& connectivity);

}
