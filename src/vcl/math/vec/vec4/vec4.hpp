#pragma once

#include "vcl/containers/buffer_stack/buffer_stack.hpp"

namespace vcl {

/** vec4 is an alias on the generic buffer_stack<float, 4>
 *  \ingroup math
*/
using vec4 = buffer_stack<float, 4>;

/** vec4 models a lightweight (x,y,z,w) 4D coordinates vector or point.
 * vec4 is a specialized-template class from a generic buffer_stack<type,N>, with type=float and N=4
 * \ingroup math
*/
template <> struct buffer_stack<float, 4> {

    // Public attributes:
    float x; /**< x-coordinate or vec4[0]  */
    float y; /**< y-coordinate or vec4[1]  */
    float z; /**< z-coordinate or vec4[2]  */
    float w; /**< w-coordinate or vec4[3]  */

    /** \name Constructors */
    ///@{
    /** Empty constructor initialize vec4=(0,0,0,0) */
    buffer_stack<float, 4>();
    /** Direct constructor.
     * vec4(x,y,z,w), or vec4{x,y,z,w}, or vec4 p = {x,y,z,w}; */
    buffer_stack<float, 4>(float x,float y,float z,float w);
    ///@}

    /** Return 4 */
    size_t size() const;

    /** \name Element access
     * \brief  Allow vec4[0/1/2/3], or vec4(0/1/2/3), or vec4.at(0/1/2/3) */
    ///@{
    const float& operator[](std::size_t index) const;
    float& operator[](std::size_t index);

    const float& operator()(std::size_t index) const;
    float& operator()(std::size_t index);

    float const& at(std::size_t index) const;
    float& at(std::size_t index);
    ///@}

    /** \name Iterators
     * Iterators are direct pointers on data */
    ///@{
    float* begin();
    float* end();
    float const* begin() const;
    float const* end() const;
    float const* cbegin() const;
    float const* cend() const;
    ///@}
};



}
