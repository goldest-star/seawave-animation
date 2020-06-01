#pragma once

#include "vcl/math/mat/mat.hpp"
#include "vcl/math/vec/vec.hpp"

namespace vcl
{

/** \brief Represent an affine transformation defined by a translation, rotation, scaling (isotropic and anisotropic)
 *
 * The equivalent internal storage is computed as the 4x4 matrix:
 * \verbatim
 *
 * (    scaling * rotation  | translation )
 * (                        |             )
 * (________________________|____________ )
 * (            0           |     1       )
 *
 * \endverbatim
 *
 *  The associated transformation T is such that p'=T(p), with
 *  p' = scaling * scaling_axis * rotation * p + translation
 *  - translation: [tx,ty,tz]
 *  - rotation: 3x3 rotation matrix
 *  - scaling: 3x3 diagonal matrix Id*scaling
 *  - scaling_axis: 3x3 diagonal matrix [sx,0,0; 0,sy,0; 0,0,sz]
 *
 * \ingroup math
*/
struct affine_transform {
    affine_transform(const vec3& translation={0,0,0},
                     const mat3& rotation=mat3::identity(),
                     const float scaling=1.0f,
                     const vec3& scaling_axis={1.0f,1.0f,1.0f});


    /** Translation (x,y,z) */
    vec3 translation;
    /** Rotation matrix 3x3 */
    mat3 rotation;
    /** Isotropic scaling */
    float scaling;
    /** Non isotropic scaling (sx, sy, sz) */
    vec3 scaling_axis;

    /** Compute the matrix associated to the affine transform */
    mat4 matrix() const;
};

/** Composition between two affine transformation.
 *  Corresponds to the multiplication of their respective matrix.
 *  \relates affine_transform
 *  \ingroup math
 */
affine_transform operator*(const affine_transform& T1, const affine_transform& T2);

}
