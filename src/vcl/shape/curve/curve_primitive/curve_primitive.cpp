#include "curve_primitive.hpp"

#include "vcl/math/transformation/transformation.hpp"

namespace vcl {

std::vector<vcl::vec3> curve_primitve_circle(int N_sample, float radius, const vcl::vec3& center, const vcl::vec3& normal)
{
    std::vector<vcl::vec3> position;
    position.resize(N_sample);

    const mat3 rotation = rotation_between_vector_mat3({0,0,1}, normal);
    for(int k=0; k<N_sample; ++k)
    {
        const float u = float(k)/(float(N_sample)-1.0f);
        const vec3 p = {radius*std::cos(2*3.14159f*u), radius*std::sin(2*3.14159f*u), 0};

        position[k] = rotation*p+center;
    }

    return position;

}

}
