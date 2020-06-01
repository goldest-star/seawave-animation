#pragma once

#include "../mesh_structure/mesh.hpp"

namespace vcl
{

/** Create a discrete triangulated grid connectivity
    Nu: Number of vertices in u direction
    Nv: Number of vertices in v direction
    periodic_u: Is the shape periodic in u direction (connect last and first vertex)
    periodic_v: Is the shape periodic in v direction (connect last and first vertex)
*/
std::vector<uint3> connectivity_grid(size_t Nu, size_t Nv, bool periodic_u, bool periodic_v);

/** Quadrangle defined by four points */
mesh mesh_primitive_quad(const vec3& p00={-0.5f,-0.5f,0.0f}, const vec3& p10={0.5f,-0.5f,0.0f}, const vec3& p11={0.5f,0.5f,0.0f}, const vec3& p01={-0.5f,0.5f,0.0});

mesh mesh_primitive_sphere(float radius=1.0f, const vec3& p0={0,0,0}, size_t Nu=20, size_t Nv=40);
mesh mesh_primitive_cylinder(float radius=0.2f, const vec3& p1={0,0,0}, const vec3& p2={0,0,1}, size_t Nu=10, size_t Nv=10, bool is_border_duplicated=false);
mesh mesh_primitive_cone(float radius=0.2f, const vec3& p_base={0,0,0}, const vec3& p_extremity={0,0,1}, size_t Nu=20, size_t Nv=10);
mesh mesh_primitive_torus(float external_radius=1.0f, float internal_radius=0.25f, const vec3& center={0,0,0}, const vec3& axis_direction={0,0,1}, size_t Nu=15, size_t Nv=30, bool is_border_duplicated = false);



mesh mesh_primitive_disc(float radius=1.0f, const vec3& p0={0,0,0}, const vec3& n={0,0,1}, size_t N=20);

/** Create a parallelepiped defined by a corner point p0, and three axis vector (u1,u2,u3).
 * Generate by default a unit cube between (-0.5,-0.5,-0.5) and (0.5,0.5,0.5) */
mesh mesh_primitive_parallelepiped(const vec3& p0={-0.5,-0.5,-0.5}, const vec3& u1={1,0,0}, const vec3& u2={0,1,0}, const vec3& u3={0,0,1});

/** Create an frame made of three (R,G,B) arrows. The frame is oriented along (x,y,z) direction, and has a unit length.
 * Radius of sphere, cylinder, and cone can be parameterized. */
mesh mesh_primitive_frame(float sphere_radius=0.15f, float cylinder_radius=0.05f, float cone_radius=0.15f, float cone_length=0.3f);

mesh mesh_primitive_grid(size_t Nu=10, size_t Nv=10, const vec3& p0={0,0,0}, const vec3& direction_u={1,0,0}, const vec3& direction_v={0,1,0});
mesh mesh_primitive_bar_grid(int Nu=10, int Nv=10, int Nw=10, const vec3& p0={-0.5,-0.5,-0.5}, const vec3& direction_u={1,0,0}, const vec3& direction_v={0,1,0}, const vec3& direction_w={0,0,1});

}
