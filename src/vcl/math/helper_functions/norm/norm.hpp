#pragma once

#include "vcl/containers/containers.hpp"

namespace vcl
{

/** \name Dot product
 * \f$a\cdot b = \sum_i a_i\,b_i\f$ */
///@{
/** \relates buffer_stack \ingroup math */
template <size_t N> float dot(buffer_stack<float,N> const& a, buffer_stack<float,N> const& b);
/** \relates buffer_stack */
template <size_t N> float dot(buffer_stack<double,N> const& a, buffer_stack<double,N> const& b);
///@}

/** \name Norm
 * Standard Euclidian norm: \f$\|v\|=\sqrt{\sum v_i^2}\f$ */
///@{
/** \relates buffer_stack  ingroup math */
template <size_t N> float norm(const buffer_stack<float,N>& v);
/** \relates buffer_stack */
template <size_t N> double norm(const buffer_stack<double,N>& v);
///@}


/** \name Normalize
 * \f$\mbox{normalize}(v)=v/\|v\|\f$ */
///@{
/** \relates buffer_stack \ingroup math */
template <size_t N> buffer_stack<float,N> normalize(const buffer_stack<float,N>& v);
/** \relates buffer_stack */
template <size_t N> buffer_stack<double,N> normalize(const buffer_stack<double,N>& v);
///@}

}


namespace vcl
{

template <size_t N> float dot(buffer_stack<float,N> const& a, buffer_stack<float,N> const& b)
{
    float res = 0.0f;
    for(size_t k=0; k<N; ++k)
        res += a[k] * b[k];
    return res;
}

template <size_t N> double dot(buffer_stack<double,N> const& a, buffer_stack<double,N> const& b)
{
    double res = 0.0;
    for(size_t k=0; k<N; ++k)
        res += a[k] * b[k];
    return res;
}

template <size_t N> float norm(const buffer_stack<float,N>& v)
{
    return std::sqrt(dot(v,v));
}
template <size_t N> float norm(const buffer_stack<double,N>& v)
{
    return std::sqrt(dot(v,v));
}

#ifdef __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
template <size_t N> buffer_stack<float,N> normalize(buffer_stack<float,N> const& v)
{
    float const n = norm(v);
    buffer_stack<float,N> u;

    if(n==0)
        u[0]=1;
    else
        u = v/n;
    return u;
}

template <size_t N> buffer_stack<double,N> normalize(buffer_stack<double,N> const& v)
{
    double const n = norm(v);
    buffer_stack<double,N> u;

    if(n==0)
        u[0]=1;
    else
        u = v/n;
    return u;
}
#ifdef __linux__
#pragma GCC diagnostic pop
#endif

}
