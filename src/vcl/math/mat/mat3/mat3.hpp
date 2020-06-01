#pragma once

#include "../mat/mat.hpp"
#include "../../vec/vec3/vec3.hpp"

#include <vector>

namespace vcl {
using mat3 = mat<3,3>;

/** \brief Lightweight 3x3 matrix inspired from GLSL mat3.
 * Specialized structure from the more generic mat<N1,N2>
 * \ingroup math
*/
template <>
struct mat<3,3> {

    /** \name Internal data */
    ///@{
    float xx,xy,xz;
    float yx,yy,yz;
    float zx,zy,zz;
    ///@}

    /** \name Constructor */
    ///@{
    /** Default initialization to identity */
    mat<3,3>();
    /** Direct constructor */
    mat<3,3>(float xx,float xy,float xz,
             float yx,float yy,float yz,
             float zx,float zy,float zz);
    /** Initialization from 3 columns vec3 */
    mat<3,3>(vec3 const& column0, vec3 const& column1, vec3 const& column2);
    ///@}

    /** Generate a zero-filled mat3 */
    static mat3 zero();
    /** Generate an identity mat3 */
    static mat3 identity();

    /** Scaling matrix
     * Return the matrix
     * \verbatim
     * (s 0 0)
     * (0 s 0)
     * (0 0 s)
     * \endverbatim
    */
    static mat3 from_scaling(float s);

    /** Scaling matrix
     * Return the matrix
     * \verbatim
     * (sx 0 0)
     * (0 sy 0)
     * (0 0 sz)
     * \endverbatim
    */
    static mat3 from_scaling(const vcl::vec3& s);

    /** \name Row/Column access */
    ///@{
    vec3 row(std::size_t offset) const;
    vec3 col(std::size_t offset) const;
    mat3& set_row(std::size_t offset, const vec3& v);
    mat3& set_col(std::size_t offset, const vec3& v);
    ///@}


    /** \name Element access */
    ///@{
    const float& operator[](std::size_t offset) const; /**< value = mat[i] */
    float& operator[](std::size_t offset);             /**< mat[i] = value */

    const float& operator()(std::size_t index1, std::size_t index2) const; /**< value = mat(x,y) */
    float& operator()(std::size_t index1, std::size_t index2);             /**< mat(x,y) = value */
    ///@}
};

/** Matrix determinant. \relates mat<3,3> \ingroup math */
float det(const mat3& m);
/** Matrix inverse. \relates mat<3,3> \ingroup math */
mat3 inverse(const mat3& m);





}
