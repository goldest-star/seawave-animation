#pragma once


#include "vcl/wrapper/glad/glad.hpp"
#include "../../mesh_structure/mesh.hpp"


namespace vcl
{

struct mesh_drawable_gpu_data {

    mesh_drawable_gpu_data();
    mesh_drawable_gpu_data(const mesh& mesh_cpu);

    /** Clear buffers */
    void clear();

    /** Dynamically update the VBO with the new vector of position
     * Warning: new_position is expected to have the same size (or less) than the initialized one */
    void update_position(const buffer<vec3>& new_position);

    /** Dynamically update the VBO with the new vector of normal
     * Warning: new_normal is expected to have the same size (or less) than the initialized one */
    void update_normal(const buffer<vec3>& new_normal);


    GLuint vao;
    unsigned int number_triangles;

    GLuint vbo_index;      // Triplet (i,j,k) of triangle index

    GLuint vbo_position;   // (x,y,z) coordinates
    GLuint vbo_normal;     // (nx,ny,nz) normals coordinates (unit length)
    GLuint vbo_color;      // (r,g,b) values
    GLuint vbo_texture_uv; // (u,v) texture coordinates
};

/** Call raw OpenGL draw */
void draw(const mesh_drawable_gpu_data& gpu_data);

}
