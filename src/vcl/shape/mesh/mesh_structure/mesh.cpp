#include "mesh.hpp"

#include "vcl/math/math.hpp"

namespace vcl
{

void mesh::push_back(const mesh& mesh_to_add)
{
    fill_empty_fields();
    mesh m = mesh_to_add;
    m.fill_empty_fields();

    const size_t N0 = position.size();
    const size_t N = m.position.size();
    for(size_t k=0; k<N; ++k )
    {
        position.push_back(m.position[k]);
        normal.push_back(m.normal[k]);
        color.push_back(m.color[k]);
        texture_uv.push_back(m.texture_uv[k]);
    }

    const size_t N_tri = m.connectivity.size();
    for(size_t k=0; k<N_tri; ++k)
    {
        uint3 index = m.connectivity[k];
        for(size_t kv=0; kv<3; ++kv)
            index[kv] += static_cast<unsigned int>(N0);

        connectivity.push_back(index);
    }

}

void mesh::fill_empty_fields()
{
    const size_t N = position.size();
    if(N==0)
        return ;

    assert_vcl( N>0, "Mesh doesn't have any vertex");
    assert_vcl( connectivity.size()>0, "Connectivity doesn't have any triangle" );

    if( normal.size()<N )
        normal = vcl::normal(position, connectivity);

    if( color.size()<N )
    {
        color.resize(N);
        for(size_t k=0; k<N; ++k)
            color[k] = vec4(1,1,1,1);
    }

    if( texture_uv.size()<N )
    {
        texture_uv.resize(N);
        for(size_t k=0; k<N; ++k)
            texture_uv[k] = vec2(0,0);
    }

}

void normal(const buffer<vec3>& position, const buffer<uint3>& connectivity, buffer<vec3>& normals,bool invert)
{

    const size_t N = position.size();
    if(normals.size() != N)
        normals.resize(N);
    else
    {
        for(size_t k=0; k<N; ++k)
            normals[k] = vec3(0,0,0);
    }

    const size_t N_tri = connectivity.size();
    for(size_t k_tri=0; k_tri<N_tri; ++k_tri)
    {
        const uint3& f = connectivity[k_tri];
        assert_vcl_no_msg(f[0]<N);
        assert_vcl_no_msg(f[1]<N);
        assert_vcl_no_msg(f[2]<N);

        const vec3& p0 = position[f[0]];
        const vec3& p1 = position[f[1]];
        const vec3& p2 = position[f[2]];

        // Compute normal of the triangle
        const vec3& p10 = normalize(p1-p0);
        const vec3& p20 = normalize(p2-p0);
        const vec3& n = normalize( cross(p10,p20) );

        // Add normal direction to all vertices of this triangle
        for(size_t k=0; k<3; ++k)
            normals[f[k]] += n;
    }

    // Normalize all normals
    for(size_t k=0; k<N; ++k)
        normals[k] = normalize(normals[k]);

    if(invert)
        for(size_t k=0; k<N; ++k)
            normals[k] = -normals[k];

}

buffer<vec3> normal(const buffer<vec3>& position, const buffer<uint3>& connectivity)
{
    buffer<vec3> normals(position.size());
    normal(position,connectivity,normals);

    return normals;

}

void mesh::fill_color_uniform(const vec3& c)
{
    fill_color_uniform(vec4(c.x,c.y,c.z,1.0f));
}
void mesh::fill_color_uniform(const vec4& c)
{
    const size_t N = position.size();
    if(color.size()!=N)
        color.resize(N);

    for(size_t k=0; k<N; ++k)
        color[k] = c;
}


vec3 center_of_mass(const mesh& shape)
{
    return center_of_mass(shape.position, shape.connectivity);
}

vec3 center_of_mass(const buffer<vec3>& position, const buffer<uint3>& connectivity)
{
    const size_t N_vertex = position.size();
    assert( N_vertex>0 );

    vec3 COM = {0,0,0};
    float total_area = 0.0f;
    const size_t N_tri = connectivity.size();
    for(size_t k_tri=0; k_tri<N_tri; ++k_tri) {
        const uint3& t = connectivity[k_tri];

        assert(t[0]<N_vertex);
        assert(t[1]<N_vertex);
        assert(t[2]<N_vertex);

        const vec3& p0 = position[t[0]];
        const vec3& p1 = position[t[1]];
        const vec3& p2 = position[t[2]];

        const float area = 0.5f * norm(cross(p1-p0,p2-p0));
        COM += area * (p0+p1+p2)/3.0f;
        total_area += area;
    }

    if( total_area>1e-8f )
        COM /= total_area;
    else
        COM = position[0];

    return COM;
}

}
