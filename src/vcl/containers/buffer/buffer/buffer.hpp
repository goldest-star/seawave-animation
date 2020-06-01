#pragma once


#include <vector>
#include <ostream>

#include "vcl/base/base.hpp"
#include <iostream>

/* ************************************************** */
/*           Header                                   */
/* ************************************************** */

namespace vcl
{

/** \brief Dynamic-sized container for numerical data
 *
 * The buffer structure is a wrapper around an std::vector with additional convenient functionalities
 * - Overloaded operators + - * / as well as common outputs
 * - Strict bound checking with operator [] and () (unless VCL_NO_DEBUG is defined)
 *
 * Buffer follows the main syntax than std::vector
 * Elements in a buffer sotred contiguously in memory (use std::vector internally)
 *
 * \ingroup container
 */
template <typename T>
struct buffer
{
    /** Internal data stored as std::vector */
    std::vector<T> data;

    /** \name Constructors
     * \brief  Follows the syntax from std::vector */
    ///@{
    buffer();                             /**< Empty buffer - no elements */
    buffer(size_t size);                  /**< Buffer with a given size */
    buffer(std::initializer_list<T> arg); /**< Inline initialization using { } */
    buffer(std::vector<T> const& arg);    /**< Direct initialization from std::vector */
    ///@}


    /** Container size similar to vector.size() */
    size_t size() const;
    /** Resize container to a new size (similar to vector.resize()) */
    void resize(size_t size);
    /** Add an element at the end of the container (similar to vector.push_back()) */
    void push_back(T const& value);
    /** Remove all elements of the container, new size is 0 (similar to vector.clear()) */
    void clear();
    /** Fill the container with the same element (from index 0 to size-1) */
    void fill(T const& value);

    /** \name Element access
     * \brief  Allows buffer[i], buffer(i), and buffer.at(i)
     * Bound checking is performed unless VCL_NO_DEBUG is defined. */
    ///@{
    T const& operator[](size_t index) const;
    T & operator[](size_t index);
    T const& operator()(size_t index) const;
    T & operator()(size_t index);
    T const& at(size_t index) const; /**< Internal call to std::vector.at */
    T & at(size_t index);            /**< Internal call to std::vector.at */
    ///@}

    /** \name Iterators
     * \brief  Iterators on buffer are compatible with STL syntax
     * allows "forall" loops (for(auto& e : buffer) {...}) */
    ///@{
    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::const_iterator end() const;
    typename std::vector<T>::const_iterator cbegin() const;
    typename std::vector<T>::const_iterator cend() const;
    ///@}
};

/** Display all elements of the buffer. \relates buffer \ingroup container */
template <typename T> std::ostream& operator<<(std::ostream& s, buffer<T> const& v);

/** Convert all elements of the buffer to a string.
 * \param buffer: the input buffer
 * \param separator: the separator between each element
 * \relates buffer
 * \ingroup container
*/
template <typename T> std::string to_string(buffer<T> const& v, std::string const& separator=" ");


/** \name Equality check
 * \brief   Check equality (element by element) between two buffers.
 * Buffers with different size are always considered as not equal.
 * Only approximated equality is performed for comprison with float (absolute value between floats) */
///@{
/** Allows to check value equality between different type (float and int for instance). \relates buffer \ingroup container */
template <typename T1, typename T2> bool is_equal(buffer<T1> const& a, buffer<T2> const& b);
/** \relates buffer */
template <typename T> bool is_equal(buffer<T> const& a, buffer<T> const& b);
///@}

/** Compute average value of all elements of the buffer. \relates buffer */
template <typename T> T average(buffer<T> const& a);


/** \name Math operators
 * \brief Common mathematical operations between buffers, and scalar or element values. */
///@{

/** \relates buffer \ingroup container */
template <typename T> buffer<T>  operator-(buffer<T> const& a);

/** \relates buffer */ template <typename T> buffer<T>& operator+=(buffer<T>& a, buffer<T> const& b);
/** \relates buffer */ template <typename T> buffer<T>& operator+=(buffer<T>& a, T const& b);
/** \relates buffer */ template <typename T> buffer<T>  operator+(buffer<T> const& a, buffer<T> const& b);
/** \relates buffer */ template <typename T> buffer<T>  operator+(buffer<T> const& a, T const& b); /**< Componentwise sum: a[i]+b */
/** \relates buffer */ template <typename T> buffer<T>  operator+(T const& a, buffer<T> const& b); /**< Componentwise sum: a+b[i] */

/** \relates buffer */ template <typename T> buffer<T>& operator-=(buffer<T>& a, buffer<T> const& b);
/** \relates buffer */ template <typename T> buffer<T>& operator-=(buffer<T>& a, T const& b);
/** \relates buffer */ template <typename T> buffer<T>  operator-(buffer<T> const& a, buffer<T> const& b);
/** \relates buffer */ template <typename T> buffer<T>  operator-(buffer<T> const& a, T const& b); /**< Componentwise substraction: a[i]-b */
/** \relates buffer */ template <typename T> buffer<T>  operator-(T const& a, buffer<T> const& b); /**< Componentwise substraction: a-b[i] */

/** \relates buffer */ template <typename T> buffer<T>& operator*=(buffer<T>& a, buffer<T> const& b);
/** \relates buffer */ template <typename T> buffer<T>  operator*(buffer<T> const& a, buffer<T> const& b);
/** \relates buffer */ template <typename T> buffer<T>& operator*=(buffer<T>& a, float b);
/** \relates buffer */ template <typename T> buffer<T>  operator*(buffer<T> const& a, float b);
/** \relates buffer */ template <typename T> buffer<T>  operator*(float a, buffer<T> const& b);

/** \relates buffer */ template <typename T> buffer<T>& operator/=(buffer<T>& a, buffer<T> const& b);
/** \relates buffer */ template <typename T> buffer<T>& operator/=(buffer<T>& a, float b);
/** \relates buffer */ template <typename T> buffer<T>  operator/(buffer<T> const& a, buffer<T> const& b);
/** \relates buffer */ template <typename T> buffer<T>  operator/(buffer<T> const& a, float b);
///@}

}



/* ************************************************** */
/*           IMPLEMENTATION                           */
/* ************************************************** */

namespace vcl
{

template <typename T>
buffer<T>::buffer()
    :data()
{}

template <typename T>
buffer<T>::buffer(size_t size)
    :data(size)
{}

template <typename T>
buffer<T>::buffer(std::initializer_list<T> arg)
    :data(arg)
{}

template <typename T>
buffer<T>::buffer(const std::vector<T>& arg)
    :data(arg)
{}

template <typename T>
size_t buffer<T>::size() const
{
    return data.size();
}

template <typename T>
void buffer<T>::resize(size_t size)
{
    data.resize(size);
}

template <typename T>
void buffer<T>::push_back(T const& value)
{
    data.push_back(value);
}

template <typename T>
void buffer<T>::clear()
{
    data.clear();
}

template <typename T>
T const& buffer<T>::operator[](size_t index) const
{
    assert_vcl(index<data.size(), "index="+str(index));
    return data[index];
}
template <typename T>
T & buffer<T>::operator[](size_t index)
{
    assert_vcl(index<data.size(), "index="+str(index));
    return data[index];
}

template <typename T>
T const& buffer<T>::operator()(size_t index) const
{
    return (*this)[index];
}

template <typename T>
T & buffer<T>::operator()(size_t index)
{
    return (*this)[index];
}

template <typename T>
T const& buffer<T>::at(size_t index) const
{
    return data.at(index);
}

template <typename T>
T & buffer<T>::at(size_t index)
{
    return data.at(index);
}

template <typename T>
void buffer<T>::fill(T const& value)
{
    size_t const N = size();
    for(size_t k=0; k<N; ++k)
        data[k] = value;
}

template <typename T>
typename std::vector<T>::iterator buffer<T>::begin()
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::iterator buffer<T>::end()
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator buffer<T>::begin() const
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::const_iterator buffer<T>::end() const
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator buffer<T>::cbegin() const
{
    return data.cbegin();
}

template <typename T>
typename std::vector<T>::const_iterator buffer<T>::cend() const
{
    return data.cend();
}

template <typename T> std::ostream& operator<<(std::ostream& s, buffer<T> const& v)
{
    std::string s_out = to_string(v);
    s << s_out;
    return s;
}
template <typename T> std::string to_string(buffer<T> const& v, std::string const& separator)
{
    return vcl::detail::to_string_container(v, separator);
}

template <typename T> T average(buffer<T> const& a)
{
    size_t const N = a.size();
    assert_vcl_no_msg(N>0);

    T value; // assume value start at zero
    for(size_t k=0; k<N; ++k)
        value += a[k];
    value /= float(N);

    return value;
}

template <typename T>
buffer<T>& operator+=(buffer<T>& a, buffer<T> const& b)
{
    assert_vcl(a.size()>0 && b.size()>0, "Size must be >0");
    assert_vcl(a.size()==b.size(), "Size do not agree");

    const size_t N = a.size();
    for(size_t k=0; k<N; ++k)
        a[k] += b[k];
    return a;
}

template <typename T>
buffer<T>& operator+=(buffer<T>& a, T const& b)
{
    assert_vcl(a.size()>0, "Size must be >0");
    const size_t N = a.size();
    for(size_t k=0; k<N; ++k)
        a[k] += b;
    return a;
}

template <typename T>
buffer<T>  operator+(buffer<T> const& a, buffer<T> const& b)
{
    buffer<T> res = a;
    res += b;
    return res;
}

template <typename T>
buffer<T>  operator+(buffer<T> const& a, T const& b)
{
    buffer<T> res = a;
    res += b;
    return res;
}

template <typename T>
buffer<T>  operator+(T const& a, buffer<T> const& b)
{
    size_t const N = b.size();
    buffer<T> res(N);
    for(size_t k=0; k<N; ++k)
        res[k] = a+b[k];
    return res;
}

template <typename T> buffer<T>  operator-(buffer<T> const& a)
{
    size_t const N = a.size();
    buffer<T> b(N);
    for(size_t k=0; k<N; ++k)
        b[k] = -a[k];
    return b;
}


template <typename T> buffer<T>& operator-=(buffer<T>& a, buffer<T> const& b)
{
    assert_vcl(a.size()>0 && b.size()>0, "Size must be >0");
    assert_vcl(a.size()==b.size(), "Size do not agree");

    const size_t N = a.size();
    for(size_t k=0; k<N; ++k)
        a[k] -= b[k];
    return a;
}
template <typename T> buffer<T>& operator-=(buffer<T>& a, T const& b)
{
    assert_vcl(a.size()>0, "Size must be >0");
    const size_t N = a.size();
    for(size_t k=0; k<N; ++k)
        a[k] -= b;
    return a;
}
template <typename T> buffer<T>  operator-(buffer<T> const& a, buffer<T> const& b)
{
    buffer<T> res = a;
    res -= b;
    return res;
}
template <typename T> buffer<T>  operator-(buffer<T> const& a, T const& b)
{
    buffer<T> res = a;
    res -= b;
    return res;
}
template <typename T> buffer<T>  operator-(T const& a, buffer<T> const& b)
{
    size_t const N = b.size();
    buffer<T> res(N);
    for(size_t k=0; k<N; ++k)
        res[k] = a-b[k];
    return res;
}


template <typename T> buffer<T>& operator*=(buffer<T>& a, buffer<T> const& b)
{
    assert_vcl(a.size()>0 && b.size()>0, "Size must be >0");
    assert_vcl(a.size()==b.size(), "Size do not agree");

    const size_t N = a.size();
    for(size_t k=0; k<N; ++k)
        a[k] *= b[k];
    return a;
}
template <typename T> buffer<T>  operator*(buffer<T> const& a, buffer<T> const& b)
{
    buffer<T> res = a;
    res *= b;
    return res;
}




template <typename T> buffer<T>& operator*=(buffer<T>& a, float b)
{
    size_t const N = a.size();
    for(size_t k=0; k<N; ++k)
        a[k] *= b;
    return a;
}
template <typename T> buffer<T>  operator*(buffer<T> const& a, float b)
{
    size_t const N = a.size();
    buffer<T> res(N);
    for(size_t k=0; k<N; ++k)
        res[k] = a[k]*b;
    return res;
}
template <typename T> buffer<T>  operator*(float a, buffer<T> const& b)
{
    size_t const N = b.size();
    buffer<T> res(N);
    for(size_t k=0; k<N; ++k)
        res[k] = a*b[k];
    return res;
}

template <typename T> buffer<T>& operator/=(buffer<T>& a, buffer<T> const& b)
{
    assert_vcl(a.size()>0 && b.size()>0, "Size must be >0");
    assert_vcl(a.size()==b.size(), "Size do not agree");

    const size_t N = a.size();
    for(size_t k=0; k<N; ++k)
        a[k] /= b[k];
    return a;
}
template <typename T> buffer<T>& operator/=(buffer<T>& a, float b)
{
    assert_vcl(a.size()>0, "Size must be >0");
    const size_t N = a.size();
    for(size_t k=0; k<N; ++k)
        a[k] /= b;
    return a;
}
template <typename T> buffer<T>  operator/(buffer<T> const& a, buffer<T> const& b)
{
    buffer<T> res = a;
    res /= b;
    return res;
}
template <typename T> buffer<T>  operator/(buffer<T> const& a, float b)
{
    buffer<T> res = a;
    res /= b;
    return res;
}



template <typename T1, typename T2> bool is_equal(buffer<T1> const& a, buffer<T2> const& b)
{
    size_t const N = a.size();
    if(b.size()!=N)
        return false;

    using vcl::is_equal;
    for(size_t k=0; k<N; ++k)
        if( is_equal(a[k],b[k])==false )
            return false;
    return true;
}
template <typename T> bool is_equal(buffer<T> const& a, buffer<T> const& b)
{
    return is_equal<T,T>(a,b);
}





}
