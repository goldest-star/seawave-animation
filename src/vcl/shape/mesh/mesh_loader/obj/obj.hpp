#pragma once

#include "../../mesh_structure/mesh.hpp"

namespace vcl
{

mesh mesh_load_file_obj(const std::string& filename);
mesh mesh_load_file_obj(const std::string& filename, buffer<buffer<int>>& vertex_correspondance);


namespace loader{

    enum class obj_type {
        vertex,                // f %d %d %d
        vertex_texture,        // f %d/%d %d/%d %d/%d
        vertex_texture_normal, // f %d/%d/%d %d/%d/%d %d/%d/%d
        vertex_normal          // f %d//%d %d//%d %d//%d
    };

    /** Simple file reader of the position connectivity assuming triangles (doesn't handle texture and normal connectivity) */
    std::vector<uint3> obj_read_connectivity(const std::string& filename);

    /** Read only vertices position from obj file */
    std::vector<vec3> obj_read_positions(const std::string& filename);
    /** Real only normals from obj file */
    std::vector<vec3> obj_read_normals(const std::string& filename);
    /** Read only texture uv coordinates from obj file */
    std::vector<vec2> obj_read_texture_uv(const std::string& filename);

    /** Read faces information from obj file given a type to read (position + [texture] + [normals])
     * Return buffer < buffer <int3> >
     *          |       |        -> index of position/texture/normals
     *          |       -> vertices of a face (can be arbitrary polygon)
     *          -> buffer of faces
     * Texture and normals are set to -1 if they are not defined
    */

    buffer<buffer<int3>> obj_read_faces(const std::string& filename, obj_type const type);
}


}
