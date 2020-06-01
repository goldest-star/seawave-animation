#pragma once

#include "../mat/mat.hpp"
#include "../../vec/vec4/vec4.hpp"



namespace vcl {
using mat3 = mat<3,3>;
using vec3 = vec<3>;

using mat4 = mat<4,4>;

/** \brief Lightweight 4x4 matrix inspired from GLSL mat4.
 * Specialized structure from the more generic mat<N1,N2>
 * \ingroup math
*/
template <> struct mat<4,4> {

    /** \name Internal data */
    ///@{
    float xx,xy,xz,xw;
    float yx,yy,yz,yw;
    float zx,zy,zz,zw;
    float wx,wy,wz,ww;
    ///@}

    /** \name Constructor */
    ///@{
    /** Default initialization to identity */
    mat<4,4>();
    /** Direct constructor */
    mat<4,4>(float xx,float xy,float xz,float xw,
             float yx,float yy,float yz,float yw,
             float zx,float zy,float zz,float zw,
             float wx,float wy,float wz,float ww);
    /** Affine transform generated from a 3x3 matrix and a translation vec3. */
    mat<4,4>(const vcl::mat3& R, const vcl::vec3& t);
    ///@}

    /** Generate identity matrix */
    static mat4 identity();
    /** Matrix filled with zeros */
    static mat4 zero();
    /** Generate standard OpenGL-type perspective matrix */
    static mat4 perspective(float angle_of_view, float image_aspect, float z_near, float z_far);

    /** Generate an affine transform from 3x3 matrix and translation
     * Return the block matrix
     * \verbatim
     * ( L00 L01 L02 | tx )
     * ( L10 L11 L12 | ty )
     * ( L20 L21 L22 | tz )
     * (  0   0   0  | 1  )
     * \endverbatim
     *
     * where L: linear part, t: translation
     */
    static mat4 from_mat3_vec3(const vcl::mat3& linear_block, const vcl::vec3& translation_block);

    /** Scaling matrix.
     * \verbatim
     * (s 0 0 0)
     * (0 s 0 0)
     * (0 0 s 0)
     * (0 0 0 1)
     * \endverbatim */
    static mat4 from_scaling(float s);

    /** caling matrix.
     * \verbatim
     * (sx 0 0 0)
     * (0 sy 0 0)
     * (0 0 sz 0)
     * (0 0 0  1)
     * \endverbatim */
    static mat4 from_scaling(const vcl::vec3& s);

    /** mat3 to mat4
     * \verbatim
     * ( m00 m01 m02 | 0 )
     * ( m10 m11 m12 | 0 )
     * ( m20 m21 m22 | 0 )
     * (  0   0   0  | 1  )
     * \endverbatim */
    static mat4 from_mat3(const vcl::mat3& m);

    /** Translation vec3 to matrix
     * \verbatim
     * (  0   0   0  | tx )
     * (  0   0   0  | ty )
     * (  0   0   0  | tz )
     * (  0   0   0  |  1 )
     *  \endverbatim */
    static mat4 from_translation(const vcl::vec3& t);

    /** \name Row/Column access */
    ///@{
    vec4 row(std::size_t offset) const;
    vec4 col(std::size_t offset) const;
    mat4& set_row(std::size_t offset, const vec4& v);
    mat4& set_col(std::size_t offset, const vec4& v);
    ///@}


    /** \name Element access */
    ///@{
    const float& operator[](std::size_t offset) const;  /**< value = mat[i] */
    float& operator[](std::size_t offset);              /**< mat[i] = value */

    const float& operator()(std::size_t index1, std::size_t index2) const; /**< value = mat(x,y) */
    float& operator()(std::size_t index1, std::size_t index2);             /**< mat(x,y) = value */
    ///@}

    /** \name Set block of the matrix (3x3 linear part / translation) */
    ///@{
    vcl::mat3 mat3() const;
    mat4& set_mat3(const vcl::mat3& m);
    vcl::vec3 vec3() const;
    mat4& set_vec3(const vcl::vec3& tr);
    ///@}


};








}
