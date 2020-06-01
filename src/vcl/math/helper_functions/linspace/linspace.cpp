#include "linspace.hpp"

namespace vcl
{

buffer<float> linspace(float x_min, float x_max, size_t N)
{
    assert_vcl(N>1, "linspace size must be > 1");

    buffer<float> b(N);
    for(size_t k=0; k<N; ++k) {
        float const u = float(k)/float(N-1);
        b[k] = (1-u)*x_min + u*x_max;
    }
    return b;
}
buffer2D<vec2> linspace(vec2 const& p_min, vec2 const& p_max, size_t2 N)
{
    size_t const Nx = N[0];
    size_t const Ny = N[1];
    assert_vcl(Nx>1, "linspace size must be > 1");
    assert_vcl(Ny>1, "linspace size must be > 1");

    buffer2D<vec2> b(N);
    for(size_t kx=0; kx<Nx; ++kx) {
        float const ux = float(kx)/float(Nx-1);
        for(size_t ky=0; ky<Ny; ++ky) {
            float const uy = float(ky)/float(Ny-1);

            b(kx,ky) = {(1-ux)*p_min.x + ux*p_max.x, (1-uy)*p_min.x + uy*p_max.x};

        }
    }
    return b;
}
buffer3D<vec3> linspace(vec3 const& p_min, vec3 const& p_max, size_t3 N)
{
    size_t const Nx = N[0];
    size_t const Ny = N[1];
    size_t const Nz = N[2];
    assert_vcl(Nx>1, "linspace size must be > 1");
    assert_vcl(Ny>1, "linspace size must be > 1");
    assert_vcl(Nz>1, "linspace size must be > 1");

    buffer3D<vec3> b(N);
    for(size_t kx=0; kx<Nx; ++kx) {
        float const ux = float(kx)/float(Nx-1);
        for(size_t ky=0; ky<Ny; ++ky) {
            float const uy = float(ky)/float(Ny-1);
            for(size_t kz=0; kz<Ny; ++kz) {
                float const uz = float(kz)/float(Nz-1);

                b(kx,ky,kz) = {(1-ux)*p_min.x + ux*p_max.x, (1-uy)*p_min.y + uy*p_max.y, (1-uz)*p_min.z + uz*p_max.z};
            }
        }
    }
    return b;
}


}
