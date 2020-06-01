#pragma once

#include "../mat/mat.hpp"
#include "../../vec/vec2/vec2.hpp"

namespace vcl {

using mat2 = mat<2,2>;


/** \brief Lightweight 2x2 matrix inspired from GLSL mat2.
 * Specialized structure from the more generic mat<N1,N2>
 * \ingroup math
*/
template <> struct mat<2,2> {

    /** \name Internal data */
    ///@{
    float xx,xy;
    float yx,yy;
    ///@}

    /** \name Constructor */
    ///@{
    mat<2,2>();                    /**< Default initialization to identity */
    mat<2,2>(float xx,float xy,
             float yx,float yy);   /**< Direct constructor */
    ///@}

    static mat2 identity(); /**< Generate an identity matrix */

    /** \name Row/Column access */
    ///@{
    vec2 row(std::size_t offset) const;
    vec2 col(std::size_t offset) const;
    mat2& set_row(std::size_t offset, const vec2& v);
    mat2& set_col(std::size_t offset, const vec2& v);
    ///@}


    /** \name Element access */
    ///@{
    const float& operator[](std::size_t offset) const; /**< value = mat[i] */
    float& operator[](std::size_t offset);             /**< mat[i] = value */

    const float& operator()(std::size_t index1, std::size_t index2) const; /**< value = mat(x,y) */
    float& operator()(std::size_t index1, std::size_t index2);             /**< mat(x,y) = value */
    ///@}

};

/** Matrix determinant. \relates mat<2,2> \ingroup math */
float det(const mat2& m);
/** Matrix inverse. \relates mat<2,2> \ingroup math */
mat2 inverse(const mat2& m);





}
