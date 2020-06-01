#pragma once

#include "vcl/containers/buffer_stack/buffer_stack.hpp"


namespace vcl {

/** vec2 is an alias on a generic buffer_stack<float, 2>
 *  \ingroup math */
using vec2 = buffer_stack<float, 2>;

/** vec2 models a lightweight (x,y) 2D coordinates vector or point.
 * vec2 is a specialized-template class from a generic buffer_stack<type,N>, with type=float and N=2
 * \ingroup math
 */
template <> struct buffer_stack<float, 2> {

    float x; /**< x-coordinate or vec2[0]  */
    float y; /**< y-coordinate or vec2[1] */

    /** \name Constructors */
    ///@{
    /** Empty constructor initialize vec2=(0,0) */
    buffer_stack<float, 2>();
    /** Direct constructor.
     * vec2(x,y), or vec2{x,y}, or vec2 p = {x,y}; */
    buffer_stack<float, 2>(float x,float y);
    ///@}

    /** Return 2 */
    size_t size() const;

    /** \name Element access
     * \brief  Allow vec2[0/1], or vec2(0/1), or vec2.at(0/1) */
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
