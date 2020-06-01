#include "affine_transform.hpp"

namespace vcl
{

affine_transform::affine_transform(const vec3& translation_arg, const mat3& rotation_arg, float scaling_arg, const vec3& scaling_axis_arg)
    :translation(translation_arg), rotation(rotation_arg), scaling(scaling_arg), scaling_axis(scaling_axis_arg)
{}

mat4 affine_transform::matrix() const
{
    return mat4::from_scaling(scaling*scaling_axis) * mat4::from_mat3(rotation) * mat4::from_translation(translation);
}

affine_transform operator*(const affine_transform& T1, const affine_transform& T2)
{
    affine_transform T;
    T.scaling      = T1.scaling * T2.scaling;
    T.scaling_axis = T1.scaling_axis * T2.scaling_axis;
    T.rotation     = T1.rotation * T2.rotation;
    T.translation  = mat3::from_scaling(T1.scaling*T1.scaling_axis)*T1.rotation*T2.translation + T1.translation;
    return T;
}


}
