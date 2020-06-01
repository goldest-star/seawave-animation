#include "mesh_primitive.hpp"

#include "vcl/math/math.hpp"

namespace vcl
{

std::vector<uint3> connectivity_grid(size_t Nu, size_t Nv, bool periodic_u, bool periodic_v)
{
    std::vector<std::array<unsigned int,3> > triangle_index;
    for(size_t ku=0; ku<Nu-1; ++ku) {
        for( size_t kv=0; kv<Nv-1; ++kv) {

            const unsigned int u00 = static_cast<unsigned int>( kv + Nv*ku );
            const unsigned int u10 = static_cast<unsigned int>( kv+1 + Nv*ku );
            const unsigned int u01 = static_cast<unsigned int>( kv + Nv*(ku+1) );
            const unsigned int u11 = static_cast<unsigned int>( kv+1 + Nv*(ku+1) );

            triangle_index.push_back({u00,u10,u11});
            triangle_index.push_back({u11,u01,u00});
        }
    }

    if( periodic_u==true ) {
        for( size_t kv=0; kv<Nv-1; ++kv ) {

            const unsigned int u00 = static_cast<unsigned int>( kv + Nv*(Nu-1) );
            const unsigned int u10 = static_cast<unsigned int>( kv+1 + Nv*(Nu-1) );
            const unsigned int u01 = static_cast<unsigned int>( kv + Nv*0 );
            const unsigned int u11 = static_cast<unsigned int>( kv+1 + Nv*0 );

            triangle_index.push_back({u00,u10,u11});
            triangle_index.push_back({u11,u01,u00});
        }
    }

    if( periodic_v==true ) {
        for( size_t ku=0; ku<Nu-1; ++ku ) {

            const unsigned int u00 = static_cast<unsigned int>( Nv-1 + Nv*ku );
            const unsigned int u10 = static_cast<unsigned int>( 0 + Nv*ku );
            const unsigned int u01 = static_cast<unsigned int>( Nv-1 + Nv*(ku+1) );
            const unsigned int u11 = static_cast<unsigned int>( 0 + Nv*(ku+1) );

            triangle_index.push_back({u00,u10,u11});
            triangle_index.push_back({u11,u01,u00});
        }
    }

    if( periodic_u==true && periodic_v==true )
    {
        const unsigned int u00 = static_cast<unsigned int>( Nv-1 + Nv*(Nu-1) );
        const unsigned int u10 = static_cast<unsigned int>( 0 + Nv*(Nu-1) );
        const unsigned int u01 = static_cast<unsigned int>( Nv-1 + Nv*0 );
        const unsigned int u11 = static_cast<unsigned int>( 0 + Nv*0 );

        triangle_index.push_back({u00,u10,u11});
        triangle_index.push_back({u11,u01,u00});
    }

    return triangle_index;

}


mesh mesh_primitive_sphere(float radius, const vec3& p0, size_t Nu, size_t Nv)
{
    assert(Nu>=4);
    assert(Nv>=4);

    mesh shape;
    for( size_t ku=0; ku<Nu; ++ku ) {
        for( size_t kv=0; kv<Nv; ++kv ) {

            // Parametric coordinates
            const float u = static_cast<float>(ku)/static_cast<float>(Nu-1);
            const float v = static_cast<float>(kv)/static_cast<float>(Nv-1);

            // Angles
            const float theta = static_cast<float>( 3.14159f*v );
            const float phi   = static_cast<float>( 2*3.14159f*u );

            // Spherical coordinates
            const float x = radius * std::sin(theta) * std::cos(phi);
            const float y = radius * std::sin(theta) * std::sin(phi);
            const float z = radius * std::cos(theta);


            const vec3 p  = {x,y,z};      // Position (centered)
            const vec3 n  = normalize(p); // Normal
            const vec2 uv = {v,u};        // Texture-coordinates

            shape.position.push_back( p+p0 ); // Add new position (with translation of the center)
            shape.normal.push_back( n );
            shape.texture_uv.push_back(uv);
        }
    }

    shape.connectivity = connectivity_grid(Nu, Nv, false, false);

    return shape;
}

mesh mesh_primitive_cylinder(float radius,  const vec3& p1, const vec3& p2, size_t Nu, size_t Nv, bool is_border_duplicated)
{
    const vec3 p12 = p2-p1;
    const float L = norm(p12);
    const vec3 dir = p12/L;
    const mat3 R = rotation_between_vector_mat3({0,0,1}, dir);

    mesh shape;
    float const Nu_interval = is_border_duplicated? float(Nu) : float(Nu-1);
    for( size_t ku=0; ku<Nu; ++ku ) {
        for( size_t kv=0; kv<Nv; ++kv ) {

            const float u = static_cast<float>(ku)/Nu_interval;
            const float v = static_cast<float>(kv)/static_cast<float>(Nv-1);

            const float theta = static_cast<float>( 2* 3.14159f * u );

            const float x = radius * std::sin(theta);
            const float y = radius * std::cos(theta);
            const float z = L * v;

            const vec3 p0 = {x,y,z};
            const vec3 p  = R*p0+p1;
            const vec3 n  = normalize(R*vec3{x,y,0.0f});
            const vec2 uv = {u,v};

            shape.position.push_back( p );
            shape.normal.push_back( n );
            shape.texture_uv.push_back(uv);
        }
    }

    shape.connectivity = connectivity_grid(Nu, Nv, is_border_duplicated, false);

    return shape;
}

mesh mesh_primitive_torus(float R, float r, const vec3& center, const vec3& axis_direction, size_t Nu, size_t Nv, bool is_border_duplicated)
{
    const mat3 Rotation = rotation_between_vector_mat3({0,0,1},axis_direction);

    mesh shape;
    float const Nu_interval = is_border_duplicated? float(Nu) : float(Nu-1);
    float const Nv_interval = is_border_duplicated? float(Nv) : float(Nv-1);
    for( size_t ku=0; ku<Nu; ++ku ) {
        for( size_t kv=0; kv<Nv; ++kv ) {

            const float u = static_cast<float>(ku)/Nu_interval;
            const float v = static_cast<float>(kv)/Nv_interval;

            const float theta = static_cast<float>( 2* 3.14159f * u );
            const float phi   = static_cast<float>( 2* 3.14159f * v );

            const vec3 p0 = {(R+r*std::cos(theta)) * std::cos(phi),
                             (R+r*std::cos(theta)) * std::sin(phi),
                             r*std::sin(theta) };

            const vec3 d_theta = {-r*std::sin(theta)*std::cos(phi), -r*std::sin(theta)*std::sin(phi), r*std::cos(theta)};
            const vec3 d_phi   = {-(R+r*std::cos(theta))*std::sin(phi), (R+r*std::cos(theta))*std::cos(phi), 0};
            const vec3 n = normalize(cross(d_phi,d_theta));

            const vec2 uv = {u,v};

            shape.position.push_back( Rotation*p0+center );
            shape.normal.push_back( n );
            shape.texture_uv.push_back(uv);
        }
    }

    shape.connectivity = connectivity_grid(Nu, Nv, is_border_duplicated, is_border_duplicated);

    return shape;
}

mesh mesh_primitive_cone(float radius,  const vec3& p1, const vec3& p2, size_t Nu, size_t Nv)
{
    const vec3 p12 = p2-p1;
    const float L = norm(p12);
    const vec3 dir = p12/L;
    const mat3 R = rotation_between_vector_mat3({0,0,1},dir);

    mesh shape;
    for( size_t ku=0; ku<Nu; ++ku ) {
        for( size_t kv=0; kv<Nv; ++kv ) {

            const float u = static_cast<float>(ku)/static_cast<float>(Nu);
            const float v = static_cast<float>(kv)/static_cast<float>(Nv-1);

            const float theta = static_cast<float>( 2* 3.14159f * u );
            const float alpha = 1.0f-v;

            const float x = alpha*radius * std::sin(theta);
            const float y = alpha*radius * std::cos(theta);
            const float z = L * v;

            const vec3 p0 = {x,y,z};
            const vec3 p = R*p0+p1;

            const vec3 normal_d = R * vec3(std::sin(theta), std::cos(theta), radius/L);
            const float normal_n = norm(normal_d);
            vec3 n = normal_d;
            if( normal_n>1e-6f )
                n /= normal_n;
            else
                n = {0,0,1};


            shape.position.push_back( p );
            shape.normal.push_back( n );
        }
    }

    shape.connectivity = connectivity_grid(Nu, Nv, true, false);

    return shape;
}


mesh mesh_primitive_frame(float sphere_radius, float cylinder_radius, float cone_radius, float cone_length)
{
    mesh sphere = mesh_primitive_sphere(sphere_radius); sphere.fill_color_uniform({1,1,1});

    mesh cylinder_x = mesh_primitive_cylinder(cylinder_radius,{0,0,0},{1.0f-cone_length,0,0}); cylinder_x.fill_color_uniform({1,0,0});
    mesh cylinder_y = mesh_primitive_cylinder(cylinder_radius,{0,0,0},{0,1.0f-cone_length,0}); cylinder_y.fill_color_uniform({0,1,0});
    mesh cylinder_z = mesh_primitive_cylinder(cylinder_radius,{0,0,0},{0,0,1.0f-cone_length}); cylinder_z.fill_color_uniform({0,0,1});

    mesh cone_x = mesh_primitive_cone(cone_radius,{1.0f-cone_length,0,0},{1.0f,0,0});  cone_x.fill_color_uniform({1,0,0});
    mesh cone_y = mesh_primitive_cone(cone_radius,{0,1.0f-cone_length,0},{0,1.0f,0});  cone_y.fill_color_uniform({0,1,0});
    mesh cone_z = mesh_primitive_cone(cone_radius,{0,0,1.0f-cone_length},{0,0,1.0f});  cone_z.fill_color_uniform({0,0,1});

    mesh m;
    m.push_back(sphere);
    m.push_back(cylinder_x);
    m.push_back(cylinder_y);
    m.push_back(cylinder_z);
    m.push_back(cone_x);
    m.push_back(cone_y);
    m.push_back(cone_z);

    return m;
}



mesh mesh_primitive_quad(const vec3& p00, const vec3& p10, const vec3& p11, const vec3& p01)
{
    // Compute normal of the quadrangle (orthogonal to the two basis vectors p00->p01 and p00->p10)
    const vec3 n = normalize(cross(normalize(p10-p00), normalize(p01-p00)));

    // Fill per-vertex buffers
    mesh quad;
    quad.position     = {p00, p10, p11, p01};         // 3D-coordinates
    quad.normal       = {n,n,n,n};                    // same normal for all vertices
    quad.texture_uv   = {{0,0}, {1,0}, {1,1}, {0,1}}; // 2D (u,v) - texture coordinates
    quad.connectivity = {{0,1,2}, {0,2,3}};           // Quadrangle made up of two triangles

    return quad;
}

mesh mesh_primitive_disc(float radius, const vec3& p0, const vec3& n, size_t N)
{
    mesh disc;

    mat3 R = rotation_between_vector_mat3({0,0,1},n);

    for(size_t k=0; k<N; ++k)
    {
        const float u = static_cast<float>(k)/(static_cast<float>(N)-1.0f);
        const float theta = 2*static_cast<float>(3.14159f)*u;
        const vec3 p = R*vec3(radius*std::cos(theta),radius*std::sin(theta),0.0f);

        disc.position.push_back(p0+p);
        disc.normal.push_back(n);

        disc.connectivity.push_back( {static_cast<unsigned int>( k ),
                                      static_cast<unsigned int>((k+1)%N),
                                      static_cast<unsigned int>( N )});
    }

    // add central point
    disc.position.push_back(p0);
    disc.normal.push_back(n);

    return disc;

}

mesh mesh_primitive_grid(size_t Nu, size_t Nv, const vec3& p0, const vec3& direction_u, const vec3& direction_v)
{
    mesh shape;

    for( size_t kv=0; kv<Nv; ++kv ) {
        for( size_t ku=0; ku<Nu; ++ku ) {
            const float u = static_cast<float>(ku)/static_cast<float>(Nu-1);
            const float v = static_cast<float>(kv)/static_cast<float>(Nv-1);

            const vec3 position = u*direction_u + v*direction_v + p0;
            const vec3 normal = normalize(cross(direction_u, direction_v));

            shape.position.push_back( position );
            shape.normal.push_back( normal );
            shape.texture_uv.push_back({u,1.0f-v});
        }
    }

    shape.connectivity = connectivity_grid(Nv, Nu, false, false);


    return shape;

}



mesh mesh_primitive_parallelepiped(const vec3& p0, const vec3& u1, const vec3& u2, const vec3& u3)
{

    const vec3 p000 = p0;
    const vec3 p100 = p0+u1;
    const vec3 p010 = p0+u2;
    const vec3 p001 = p0+u3;
    const vec3 p110 = p0+u1+u2;
    const vec3 p101 = p0+u1+u3;
    const vec3 p011 = p0+u2+u3;
    const vec3 p111 = p0+u1+u2+u3;

    mesh shape;
    shape.position = {p000, p100, p110, p010,
                      p100, p101, p111, p110,
                      p110, p111, p011, p010,
                      p000, p001, p011, p010,
                      p000, p001, p101, p100,
                      p101, p001, p011, p111};


    const vec3 n1 = normalize(u1);
    const vec3 n2 = normalize(u2);
    const vec3 n3 = normalize(u3);

    shape.normal = {-n3, -n3, -n3, -n3,
                    n1, n1, n1, n1,
                    n2, n2, n2, n2,
                    -n1, -n1, -n1, -n1,
                    -n2, -n2, -n2, -n2,
                    n3, n3, n3, n3};

    shape.connectivity = {{0,1,2}, {0,2,3},
                          {4,5,6}, {4,6,7},
                          {8,9,10}, {8,10,11},
                          {12,13,14}, {12,14,15},
                          {16,17,18}, {16,18,19},
                          {20,21,22}, {20,22,23}};

    return shape;
}

mesh mesh_primitive_bar_grid(int Nu, int Nv , int Nw, const vec3& p0, const vec3& direction_u, const vec3& direction_v, const vec3& direction_w)
{
    const vec3 p1 = p0 + direction_u+direction_v+direction_w;

    mesh shape = mesh_primitive_grid(Nv,Nu, p0, direction_v, direction_u);
    shape.push_back(mesh_primitive_grid(Nu,Nw, p0, direction_u, direction_w));
    shape.push_back(mesh_primitive_grid(Nw,Nv, p0, direction_w, direction_v));

    shape.push_back(mesh_primitive_grid(Nu,Nv, p1, -direction_u, -direction_v));
    shape.push_back(mesh_primitive_grid(Nw,Nu, p1, -direction_w, -direction_u));
    shape.push_back(mesh_primitive_grid(Nv,Nw, p1, -direction_v, -direction_w));

    return shape;

}


}
