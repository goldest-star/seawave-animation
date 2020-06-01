#pragma once

#include "vcl/base/base.hpp"
#include <array>

namespace vcl
{

/** Base class for small fixed-size vectors (vec3, mat3, etc.).
 * buffer_stack structure is a generic fixed-size vector, essentially equivalent to a std::array.
 * In addition to std::array syntax, buffer_stack provides extra convenient functions for numerical vectors
 * \ingroup container
*/
template <typename T, size_t N>
struct buffer_stack
{
    /** Internal data is a std::array of float values */
    std::array<T,N> data;

    /** Size of the buffer (N - known at compile time) */
    std::size_t size() const;

    /** Fill all data with the given value */
    void fill(T const& value);

    /** \name Element access
     * \brief  Allows buffer[i], buffer(i), and buffer.at(i)
     * Bound checking is performed unless VCL_NO_DEBUG is defined. */
    ///@{
    T const& operator[](std::size_t index) const;
    T& operator[](std::size_t index);

    T const& operator()(std::size_t index) const;
    T& operator()(std::size_t index);

    T const& at(std::size_t index) const;
    T& at(std::size_t index);
    ///@}

    /** \name Iterators
     * \brief  Iterators compatible with STL syntax and std::array */
    ///@{
    typename std::array<T,N>::iterator begin();
    typename std::array<T,N>::iterator end();
    typename std::array<T,N>::const_iterator begin() const;
    typename std::array<T,N>::const_iterator end() const;
    typename std::array<T,N>::const_iterator cbegin() const;
    typename std::array<T,N>::const_iterator cend() const;
    ///@}

};


/** Display all elements of the buffer. \relates buffer_stack \ingroup container */
template <typename T, size_t N> std::ostream& operator<<(std::ostream& s, buffer_stack<T,N> const& v);

/** Convert all elements of the buffer to a string.
 * \param buffer: the input buffer
 * \param separator: the separator between each element
 * \relates buffer_stack
 * \ingroup container */
template <typename T, size_t N> std::string to_string(buffer_stack<T,N> const& v, std::string const& separator=" ");

/** \name Equality check
 * \brief   Check equality (element by element) between two buffers.
 * Buffers with different size are always considered as not equal.
 * Only approximated equality is performed for comprison with float (absolute value between floats) */
///@{
/** \relates buffer_stack \ingroup container */
template <typename T1, typename T2, size_t N1, size_t N2> bool is_equal(buffer_stack<T1,N1> const& a, buffer_stack<T2,N2> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> bool is_equal(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b);
///@}

/** Compute average value of all elements of the buffer. \relates buffer_stack \ingroup container */
template <typename T, size_t N> T average(buffer_stack<T,N> const& a);

/** \name Math operators
 * \brief Common mathematical operations between buffers, and scalar or element values. */
///@{
/** \relates buffer_stack \ingroup container */
template <typename T, size_t N> buffer_stack<T,N>  operator-(buffer_stack<T,N> const& a);

/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>& operator+=(buffer_stack<T,N>& a, buffer_stack<T,N> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>& operator+=(buffer_stack<T,N>& a, T const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator+(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator+(buffer_stack<T,N> const& a, T const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator+(T const& a, buffer_stack<T,N> const& b);

/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>& operator-=(buffer_stack<T,N>& a, buffer_stack<T,N> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>& operator-=(buffer_stack<T,N>& a, T const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator-(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator-(buffer_stack<T,N> const& a, T const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator-(T const& a, buffer_stack<T,N> const& b);

/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>& operator*=(buffer_stack<T,N>& a, buffer_stack<T,N> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>& operator*=(buffer_stack<T,N>& a, float b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator*(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator*(buffer_stack<T,N> const& a, float b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator*(float a, buffer_stack<T,N> const& b);

/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>& operator/=(buffer_stack<T,N>& a, buffer_stack<T,N> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>& operator/=(buffer_stack<T,N>& a, float b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator/(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator/(buffer_stack<T,N> const& a, float b);
/** \relates buffer_stack */
template <typename T, size_t N> buffer_stack<T,N>  operator/(float a, buffer_stack<T,N> const& b);
///@}

}




/***********************************************/
/*           Template implementation           */
/***********************************************/



namespace vcl
{

template <typename T, size_t N> std::size_t buffer_stack<T,N>::size() const
{
    return N;
}


template <typename T, size_t N> void buffer_stack<T,N>::fill(T const& value)
{
    for(size_t k=0; k<N; ++k)
        data[k] = value;
}

template <typename T, size_t N> T const& buffer_stack<T,N>::operator[](std::size_t const index) const
{
    assert_vcl(index<data.size(), "index="+str(index));
    return data[index];
}
template <typename T, size_t N> T& buffer_stack<T,N>::operator[](std::size_t const index)
{
    assert_vcl(index<data.size(), "index="+str(index));
    return data[index];
}


template <typename T, size_t N> T const& buffer_stack<T,N>::operator()(std::size_t index) const
{
    return (*this)[index];
}

template <typename T, size_t N> T& buffer_stack<T,N>::operator()(std::size_t index)
{
    return (*this)[index];
}


template <typename T, size_t N> T const& buffer_stack<T,N>::at(std::size_t index) const
{
    return data.at(index);
}

template <typename T, size_t N> T& buffer_stack<T,N>::at(std::size_t index)
{
    return data.at(index);
}

template <typename T, size_t N> typename std::array<T,N>::iterator buffer_stack<T,N>::begin()
{
    return data.begin();
}
template <typename T, size_t N> typename std::array<T,N>::iterator buffer_stack<T,N>::end()
{
    return data.end();
}
template <typename T, size_t N> typename std::array<T,N>::const_iterator buffer_stack<T,N>::begin() const
{
    return data.begin();
}
template <typename T, size_t N> typename std::array<T,N>::const_iterator buffer_stack<T,N>::end() const
{
    return data.end();
}
template <typename T, size_t N> typename std::array<T,N>::const_iterator buffer_stack<T,N>::cbegin() const
{
    return data.cbegin();
}
template <typename T, size_t N> typename std::array<T,N>::const_iterator buffer_stack<T,N>::cend() const
{
    return data.cend();
}


template <typename T, size_t N> std::ostream& operator<<(std::ostream& s, buffer_stack<T,N> const& v)
{
    std::string s_out = to_string(v);
    s << s_out;
    return s;
}
template <typename T, size_t N> std::string to_string(buffer_stack<T,N> const& v, std::string const& separator)
{
    return vcl::detail::to_string_container(v, separator);
}

template <typename T1, typename T2, size_t N1, size_t N2> bool is_equal(buffer_stack<T1,N1> const& a, buffer_stack<T2,N2> const& b)
{
    if(N1!=N2)
        return false;
    for(size_t k=0; k<N1; ++k)
        if( is_equal(a[k],b[k])==false )
            return false;
    return true;
}

template <typename T, size_t N> bool is_equal(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b)
{
    return is_equal<T,T,N,N>(a,b);
}


template <typename T, size_t N> T average(buffer_stack<T,N> const& a)
{
    static_assert(N>0, "Cannot compute average of empty buffer");

    T value {}; // assume value start at zero
    for(size_t k=0; k<N; ++k)
        value += a[k];
    value /= float(N);

    return value;
}


template <typename T, size_t N> buffer_stack<T,N>  operator-(buffer_stack<T,N> const& a)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = -a[k];
    return res;
}

template <typename T, size_t N> buffer_stack<T,N>& operator+=(buffer_stack<T,N>& a, buffer_stack<T,N> const& b)
{
    for(size_t k=0; k<N; ++k)
        a[k] += b[k];
    return a;
}
template <typename T, size_t N> buffer_stack<T,N>& operator+=(buffer_stack<T,N>& a, T const& b)
{
    for(size_t k=0; k<N; ++k)
        a[k] += b;
    return a;
}
template <typename T, size_t N> buffer_stack<T,N>  operator+(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] + b[k];
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>  operator+(buffer_stack<T,N> const& a, T const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] + b;
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>  operator+(T const& a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a + b[k];
    return res;
}

template <typename T, size_t N> buffer_stack<T,N>& operator-=(buffer_stack<T,N>& a, buffer_stack<T,N> const& b)
{
    for(size_t k=0; k<N; ++k)
        a[k] -= b[k];
    return a;
}
template <typename T, size_t N> buffer_stack<T,N>& operator-=(buffer_stack<T,N>& a, T const& b)
{
    for(size_t k=0; k<N; ++k)
        a[k] -= b;
    return a;
}
template <typename T, size_t N> buffer_stack<T,N>  operator-(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] - b[k];
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>  operator-(buffer_stack<T,N> const& a, T const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] - b;
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>  operator-(T const& a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a - b[k];
    return res;
}

template <typename T, size_t N> buffer_stack<T,N>& operator*=(buffer_stack<T,N>& a, buffer_stack<T,N> const& b)
{
    for(size_t k=0; k<N; ++k)
        a[k] *= b[k];
    return a;
}
template <typename T, size_t N> buffer_stack<T,N>& operator*=(buffer_stack<T,N>& a, float b)
{
    for(size_t k=0; k<N; ++k)
        a[k] *= b;
    return a;
}
template <typename T, size_t N> buffer_stack<T,N>  operator*(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] * b[k];
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>  operator*(buffer_stack<T,N> const& a, float b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] * b;
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>  operator*(float a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a * b[k];
    return res;
}

template <typename T, size_t N> buffer_stack<T,N>& operator/=(buffer_stack<T,N>& a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] / b[k];
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>& operator/=(buffer_stack<T,N>& a, float b)
{
    for(size_t k=0; k<N; ++k)
        a[k] /= b;
    return a;
}
template <typename T, size_t N> buffer_stack<T,N>  operator/(buffer_stack<T,N> const& a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] / b[k];
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>  operator/(buffer_stack<T,N> const& a, float b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a[k] / b;
    return res;
}
template <typename T, size_t N> buffer_stack<T,N>  operator/(float a, buffer_stack<T,N> const& b)
{
    buffer_stack<T,N> res;
    for(size_t k=0; k<N; ++k)
        res[k] = a / b[k];
    return res;
}







}
