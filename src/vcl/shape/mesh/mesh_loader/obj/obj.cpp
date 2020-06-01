#ifdef __linux__
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif 

#include "obj.hpp"

#include "vcl/base/base.hpp"

#include <map>

#include <fstream>
#include <sstream>

namespace vcl
{


// Comparator of triplet of integer for std::map
struct comparator_int3 {
    long int N; // number of vertices

    comparator_int3(int N_arg=0) :N(N_arg) {}

    // compute a<b
    bool operator()(int3 const& a, int3 const& b) const
    {
        long int const offset_a = long(a[0]) + N*(long(a[1]) + N*long(a[2]));
        long int const offset_b = long(b[0]) + N*(long(b[1]) + N*long(b[2]));
        return offset_a < offset_b;
    }
};


static buffer<buffer_stack<int3,3>> triangulate_faces(buffer<buffer<int3>> faces);


static std::pair<mesh, std::map<int3, int, comparator_int3>>
    make_unique_parameter_per_value(buffer<vec3> const& positions,
                                    buffer<vec2> const& texture_uv,
                                    buffer<vec3> const& normals,
                                    buffer<buffer_stack<int3,3>> const& faces,
                                    loader::obj_type const type);


mesh mesh_load_file_obj(const std::string& filename)
{
     buffer<buffer<int>> vertex_correspondance;
     mesh m = mesh_load_file_obj(filename, vertex_correspondance);
     return m;
}
mesh mesh_load_file_obj(const std::string& filename, buffer<buffer<int> >& vertex_correspondance)
{
    assert_file_exist(filename);

    // Load parameters
    buffer<vec3> positions = loader::obj_read_positions(filename);
    buffer<vec2> texture_uv = loader::obj_read_texture_uv(filename);
    buffer<vec3> normals = loader::obj_read_normals(filename);

    assert_vcl(positions.size()>0, str("File ")+filename+" has 0 vertices");

    // set obj type
    loader::obj_type type = loader::obj_type::vertex;
    if(texture_uv.size()>0 && normals.size()>0)
        type = loader::obj_type::vertex_texture_normal;
    else if( texture_uv.size()>0 )
        type = loader::obj_type::vertex_texture;
    else if( normals.size()>0 )
        type = loader::obj_type::vertex_normal;

    // Load connectivity and triangulate
    buffer<buffer_stack<int3,3>> faces = triangulate_faces( loader::obj_read_faces(filename, type) );

    // Set unique per-vertex value for texture and normals (duplicate vertices if necessary)
    mesh m;
    std::map<int3, int, comparator_int3> connectivity_map;
    std::tie(m,connectivity_map) = make_unique_parameter_per_value(positions, texture_uv, normals, faces, type);

    // Retrieve correspondance between initial vertices in files and new ones
    vertex_correspondance.resize(positions.size());
    for(auto const& it : connectivity_map)
    {
        int const vertex_in = it.first[0];
        int const vertex_out = it.second;

        vertex_correspondance[vertex_in].push_back(vertex_out);
    }

    return m;
}


buffer<buffer_stack<int3,3>> triangulate_faces(buffer<buffer<int3>> faces)
{
    buffer<buffer_stack<int3,3>> faces_triangulation;
    size_t const N_face = faces.size();
    for(size_t k_face=0; k_face<N_face; ++k_face)
    {
        buffer<int3> const& current_polygon = faces[k_face];
        int const N_polygon = int(current_polygon.size());

        for(int k=0; k<N_polygon-2; ++k) {
            // triangulation
            int3 const& f0 = current_polygon[0];
            int3 const& f1 = current_polygon[k+1];
            int3 const& f2 = current_polygon[k+2];

            faces_triangulation.push_back({f0,f1,f2});
        }
    }
    return faces_triangulation;
}

std::pair<mesh, std::map<int3, int, comparator_int3>>
    make_unique_parameter_per_value(buffer<vec3> const& positions,
                                    buffer<vec2> const& texture_uv,
                                    buffer<vec3> const& normals,
                                    buffer<buffer_stack<int3,3>> const& faces,
                                    loader::obj_type const type)
{
    mesh m;
    comparator_int3 comparator(int(positions.size()));
    std::map<int3, int, comparator_int3> connectivity_map(comparator); // stores map between original face index and final offset


    size_t const N_triangle = faces.size();
    for(size_t k_triangle=0; k_triangle<N_triangle; ++k_triangle)
    {
        buffer_stack<int3,3> const& tri = faces[k_triangle];
        uint3 new_triangle_index;
        for(int k=0; k<3; ++k)
        {
            int3 const& index = tri[k];
            auto const it = connectivity_map.find( index );
            if( it==connectivity_map.end() ) {

                size_t const offset = m.position.size();
                connectivity_map[index] = int(offset);
                new_triangle_index[k] = int(offset);

                int const idx_position = index[0];

                assert_vcl_no_msg( idx_position<int(positions.size()));
                m.position.push_back( positions[idx_position] );

                if(type==loader::obj_type::vertex_texture_normal || type==loader::obj_type::vertex_texture) {
                    int const idx_uv = index[1];
                    assert_vcl_no_msg( idx_uv<int(texture_uv.size()) );
                    m.texture_uv.push_back( texture_uv[ idx_uv ] );
                }
                if(type==loader::obj_type::vertex_texture_normal || type==loader::obj_type::vertex_normal) {
                    int const idx_normal = index[2];
                    assert_vcl_no_msg( idx_normal<int(normals.size()) );
                    m.normal.push_back( normals[index[2]] );
                }

            }
            else
                new_triangle_index[k] = it->second;
        }
        m.connectivity.push_back(new_triangle_index);
    }

    return {m, connectivity_map};
}


namespace loader{
std::vector<vec3> obj_read_positions(const std::string& filename)
{
    assert_file_exist(filename);
    std::vector<vec3> positions;

    std::ifstream stream(filename);
    assert_vcl(stream.is_open(), "Cannot open file "+str(filename));
    while(stream.good()) {
        std::string buffer;
        std::getline(stream,buffer);
        if( buffer.size()>0 )
        {
            std::stringstream tokens_buffer(buffer);
            std::string first_word;
            tokens_buffer >> first_word;
            if( first_word.size()>0 && first_word[0]!='#' ) {
                if( first_word=="v" ) {
                    vec3 p;
                    tokens_buffer >> p.x >> p.y >> p.z;
                    positions.push_back(p);
                }
            }
        }
    }
    stream.close();
    assert_vcl(!stream.is_open(), "Cannot close file "+str(filename));
    return positions;
}

std::vector<vec3> obj_read_normals(const std::string& filename)
{
    assert_file_exist(filename);
    std::vector<vec3> normals;

    std::ifstream stream(filename);
    assert_vcl(stream.is_open(), "Cannot open file "+str(filename));
    while(stream.good()) {
        std::string buffer;
        std::getline(stream,buffer);
        if( buffer.size()>0 )
        {
            std::stringstream tokens_buffer(buffer);
            std::string first_word;
            tokens_buffer >> first_word;
            if( first_word.size()>0 && first_word[0]!='#' ) {
                if( first_word=="vn" ) {
                    vec3 n;
                    tokens_buffer >> n.x >> n.y >> n.z;
                    normals.push_back(n);
                }
            }
        }
    }
    stream.close();
    assert_vcl(!stream.is_open(), "Cannot close file "+str(filename));
    return normals;
}

std::vector<vec2> obj_read_texture_uv(const std::string& filename)
{
    assert_file_exist(filename);
    std::vector<vec2> texture_uv;

    std::ifstream stream(filename);
    assert_vcl(stream.is_open(), "Cannot open file "+str(filename));
    while(stream.good()) {
        std::string buffer;
        std::getline(stream,buffer);
        if( buffer.size()>0 )
        {
            std::stringstream tokens_buffer(buffer);
            std::string first_word;
            tokens_buffer >> first_word;
            if( first_word.size()>0 && first_word[0]!='#' ) {
                if( first_word=="vt" ) {
                    vec2 uv;
                    tokens_buffer >> uv.x >> uv.y;
                    texture_uv.push_back( {uv.x, 1.0f-uv.y} );
                }
            }
        }
    }
    stream.close();
    assert_vcl(!stream.is_open(), "Cannot close file "+str(filename));
    return texture_uv;
}


std::vector<uint3> obj_read_connectivity(const std::string& filename)
{
    assert_file_exist(filename);

    std::vector<uint3> connectivity;

    // Open file
    std::ifstream stream(filename);
    assert_vcl(stream.is_open(), "Cannot open file "+str(filename));


    while(stream.good())
    {
        std::string buffer;
        std::getline(stream,buffer);

        if( buffer.size()>0 )
        {
            std::stringstream tokens_buffer(buffer);
            std::string first_word;
            tokens_buffer >> first_word;

            if( first_word.size()>0 && first_word[0]!='#' )
            {
                if(first_word=="f")
                {
                    std::array<std::string,3> s;
                    tokens_buffer >> s[0] >> s[1] >> s[2];

                    uint3 f;
                    for(int k=0; k<3; ++k)
                        f[k] = std::stoi(s[k])-1;

                    connectivity.push_back(f);
                }
            }
        }
    }



    stream.close();
    assert_vcl(!stream.is_open(), "Cannot close file "+str(filename));

    return connectivity;
}

int3 extract_face_index(std::string const& word, obj_type const type)
{
    int3 indices = {0,0,0};


    if(type == obj_type::vertex)
        sscanf(word.c_str(), "%d", &indices[0]);
    else if( type==obj_type::vertex_texture )
        sscanf(word.c_str(), "%d/%d", &indices[0], &indices[1]);
    else if( type==obj_type::vertex_normal )
        sscanf(word.c_str(), "%d//%d", &indices[0], &indices[2]);
    else if( type==obj_type::vertex_texture_normal )
        sscanf(word.c_str(), "%d/%d/%d", &indices[0], &indices[1], &indices[2]);

    for(int k=0; k<3; ++k)
        indices[k]--;    // obj indices starts at 1

    return indices;

}


buffer<buffer<int3>> obj_read_faces(const std::string& filename, obj_type const type)
{
    assert_file_exist(filename);
    buffer<buffer<int3>> faces;

    std::ifstream stream(filename);
    assert_vcl(stream.is_open(), "Cannot open file "+str(filename));
    while(stream.good()) {
        std::string buffer;
        std::getline(stream,buffer);
        if( buffer.size()>0 )
        {
            std::stringstream tokens_buffer(buffer);
            std::string first_word;
            tokens_buffer >> first_word;
            if( first_word.size()>0 && first_word[0]!='#' ) {
                if( first_word=="f" ) {

                    std::string word;
                    vcl::buffer<int3> current_face;
                    while(tokens_buffer) {
                        tokens_buffer >> word;

                        if(tokens_buffer){
                            int3 face_index = extract_face_index(word, type);
                            current_face.push_back(face_index);
                        }
                    }
                    faces.push_back(current_face);

                }
            }
        }
    }
    stream.close();
    assert_vcl(!stream.is_open(), "Cannot close file "+str(filename));

    return faces;
}


}

}
