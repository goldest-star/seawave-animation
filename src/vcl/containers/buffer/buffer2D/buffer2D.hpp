#pragma once

#include "../buffer/buffer.hpp"

#include "vcl/base/base.hpp"


/* ************************************************** */
/*           Header                                   */
/* ************************************************** */

namespace vcl
{

/** \brief Container for 2D-grid like structure storing numerical element
 *
 * The buffer2D structure provide convenient access for 2D-grid organization where an element can be queried as buffer2D(i,j).
 * Elements of buffer2D are stored contiguously in memory and remain fully compatible with std::vector and pointers.
 *
 * \ingroup container
 */
template <typename T>
struct buffer2D
{
    /** 2D dimension (Nx,Ny) of the container */
    size_t2 dimension;
    /** Internal storage as a 1D buffer */
    buffer<T> data;

    /** \name Constructors
     * \brief  Follows the syntax from std::vector */
    ///@{
    buffer2D();                              /**< Empty buffer - no elements */
    buffer2D(size_t size);                   /**< Build a buffer2D of squared dimension (size,size) */
    buffer2D(size_t2 const& size);           /**< Build a buffer2D with specified dimension */
    buffer2D(size_t size_1, size_t size_2);  /**< Build a buffer2D with specified dimension */
    ///@}


    /** Remove all elements from the buffer2D */
    void clear();
    /** Total number of elements size = dimension[0] * dimension[1] */
    size_t size() const;
    /** Fill all elements of the buffer2D with the same element*/
    void fill(T const& value);

    /** \name Resizing buffer2D */
    ///@{
    void resize(size_t size);         /**< Create a squared buffer2D of dimension (size,size) */
    void resize(size_t2 const& size);
    void resize(size_t size_1, size_t size_2);
    ///@}

    /** \name Element access
     * Bound checking is performed unless VCL_NO_DEBUG is defined. */
    ///@{
    T const& operator[](size_t const& index) const; /**< Index as an offset in the 1D structure */
    T & operator[](size_t const& index);            /**< Index as an offset in the 1D structure */
    T const& operator()(size_t const& index) const; /**< Index as an offset in the 1D structure */
    T & operator()(size_t const& index);            /**< Index as an offset in the 1D structure */

    T const& operator[](size_t2 const& index) const; /**< buffer2D[ {x,y} ] */
    T & operator[](size_t2 const& index);            /**< buffer2D[ {x,y} ] */
    T const& operator()(size_t2 const& index) const; /**< buffer2D( {x,y} ) */
    T & operator()(size_t2 const& index);            /**< buffer2D( {x,y} ) */
    T const& operator()(size_t k1, size_t k2) const; /**< buffer2D(x, y) */
    T & operator()(size_t k1, size_t k2);            /**< buffer2D(x, y) */
    ///@}

    /** \name Iterators
     * \brief 1D-type iterators on buffer2D are compatible with STL syntax
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
template <typename T> std::ostream& operator<<(std::ostream& s, buffer2D<T> const& v);

/** Convert all elements of the buffer to a string.
 * \param buffer: the input buffer
 * \param separator: the separator between each element
 * \relates buffer
 * \ingroup container */
template <typename T> std::string to_string(buffer2D<T> const& v, std::string const& separator=" ");

/** \name Math operators
 * \brief Common mathematical operations between buffers, and scalar or element values. */
///@{
/** \relates buffer \ingroup container */
template <typename T> buffer2D<T>& operator+=(buffer2D<T>& a, buffer2D<T> const& b);

/** \relates buffer */ template <typename T> buffer2D<T>& operator+=(buffer2D<T>& a, T const& b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator+(buffer2D<T> const& a, buffer2D<T> const& b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator+(buffer2D<T> const& a, T const& b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator+(T const& a, buffer2D<T> const& b);

/** \relates buffer */ template <typename T> buffer2D<T>& operator-=(buffer2D<T>& a, buffer2D<T> const& b);
/** \relates buffer */ template <typename T> buffer2D<T>& operator-=(buffer2D<T>& a, T const& b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator-(buffer2D<T> const& a, buffer2D<T> const& b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator-(buffer2D<T> const& a, T const& b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator-(T const& a, buffer2D<T> const& b);

/** \relates buffer */ template <typename T> buffer2D<T>& operator*=(buffer2D<T>& a, buffer2D<T> const& b);
/** \relates buffer */ template <typename T> buffer2D<T>& operator*=(buffer2D<T>& a, float b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator*(buffer2D<T> const& a, buffer2D<T> const& b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator*(buffer2D<T> const& a, float b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator*(float a, buffer2D<T> const& b);

/** \relates buffer */ template <typename T> buffer2D<T>& operator/=(buffer2D<T>& a, buffer2D<T> const& b);
/** \relates buffer */ template <typename T> buffer2D<T>& operator/=(buffer2D<T>& a, float b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator/(buffer2D<T> const& a, buffer2D<T> const& b);
/** \relates buffer */ template <typename T> buffer2D<T>  operator/(buffer2D<T> const& a, float b);
///@}

/** Direct build a buffer2D from a given 1D-buffer and its 2D-dimension
 * \note: the size of the 1D-buffer must satisfy arg.size = size_1 * size_2
 * \relates buffer
 * \ingroup container
*/
template <typename T> buffer2D<T> buffer2D_from_vector(buffer<T> const& arg, size_t size_1, size_t size_2);

}



/* ************************************************** */
/*           IMPLEMENTATION                           */
/* ************************************************** */

namespace vcl
{

static inline size_t offset(size_t2 const& k, size_t2 const& dim)
{
    return k[0]+dim[0]*k[1];
}


template <typename T>
buffer2D<T>::buffer2D()
    :dimension(size_t2{0,0}),data()
{}

template <typename T>
buffer2D<T>::buffer2D(size_t size)
    :dimension({size,size}),data(size*size)
{}

template <typename T>
buffer2D<T>::buffer2D(size_t2 const& size)
    :dimension(size),data(size[0]*size[1])
{}

template <typename T>
buffer2D<T>::buffer2D(size_t size_1, size_t size_2)
    :dimension({size_1,size_2}),data(size_1*size_2)
{}



template <typename T>
size_t buffer2D<T>::size() const
{
    return dimension[0]*dimension[1];
}

template <typename T>
void buffer2D<T>::resize(size_t size)
{
    resize(size,size);
}

template <typename T>
void buffer2D<T>::resize(size_t2 const& size)
{
    dimension = size;
    data.resize(size[0]*size[1]);
}

template <typename T>
void buffer2D<T>::resize(size_t size_1, size_t size_2)
{
    dimension = {size_1,size_2};
    resize({size_1,size_2});
}

template <typename T>
void buffer2D<T>::fill(T const& value)
{
    data.fill(value);
}


template <typename T>
T const& buffer2D<T>::operator[](size_t const& index) const
{
    assert_vcl(index<data.size(), "Index="+str(index));
    return data[index];
}

template <typename T>
T & buffer2D<T>::operator[](size_t const& index)
{
    assert_vcl(index<data.size(), "Index="+str(index));
    return data[index];
}

template <typename T>
T const& buffer2D<T>::operator()(size_t const& index) const
{
    return (*this)[index];
}

template <typename T>
T & buffer2D<T>::operator()(size_t const& index)
{
    return (*this)[index];
}



template <typename T>
T const& buffer2D<T>::operator[](size_t2 const& index) const
{
    assert_vcl(index[0]<dimension[0], "index=("+str(index)+"), dimension=("+str(dimension)+")");
    assert_vcl(index[1]<dimension[1], "index=("+str(index)+"), dimension=("+str(dimension)+")");

    size_t const k = offset(index,dimension);
    assert_vcl(k<data.size(), "Should never happen");

    return data[k];
}

template <typename T>
T & buffer2D<T>::operator[](size_t2 const& index)
{
    assert_vcl(index[0]<dimension[0], "index=("+str(index)+"), dimension=("+str(dimension)+")");
    assert_vcl(index[1]<dimension[1], "index=("+str(index)+"), dimension=("+str(dimension)+")");

    size_t const k = offset(index,dimension);
    assert_vcl(k<data.size(), "Should never happen");

    return data[k];
}



template <typename T>
T const& buffer2D<T>::operator()(size_t2 const& index) const
{
    return (*this)[index];
}

template <typename T>
T & buffer2D<T>::operator()(size_t2 const& index)
{
    return (*this)[index];
}

template <typename T>
T const& buffer2D<T>::operator()(size_t k1, size_t k2) const
{
    return (*this)({k1,k2});
}

template <typename T>
T & buffer2D<T>::operator()(size_t k1, size_t k2)
{
    return (*this)({k1,k2});
}

template <typename T>
typename std::vector<T>::iterator buffer2D<T>::begin()
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::iterator buffer2D<T>::end()
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator buffer2D<T>::begin() const
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::const_iterator buffer2D<T>::end() const
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator buffer2D<T>::cbegin() const
{
    return data.cbegin();
}

template <typename T>
typename std::vector<T>::const_iterator buffer2D<T>::cend() const
{
    return data.cend();
}

template <typename T> std::ostream& operator<<(std::ostream& s, buffer2D<T> const& v)
{
    return s << v.data;
}
template <typename T> std::string to_string(buffer2D<T> const& v, std::string const& separator)
{
    return to_string(v.data, separator);
}


template <typename T> buffer2D<T>& operator+=(buffer2D<T>& a, buffer2D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data += b.data;
}
template <typename T> buffer2D<T>& operator+=(buffer2D<T>& a, T const& b)
{
    a.data += b;
}
template <typename T> buffer2D<T>  operator+(buffer2D<T> const& a, buffer2D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    buffer2D<T> res(a.dimension);
    res.data = a.data+b.data;
    return res;

}
template <typename T> buffer2D<T>  operator+(buffer2D<T> const& a, T const& b)
{
    buffer2D<T> res(a.dimension);
    res.data = a.data+b;
    return res;
}
template <typename T> buffer2D<T>  operator+(T const& a, buffer2D<T> const& b)
{
    buffer2D<T> res(b.dimension);
    res.data = a + b.data;
    return res;
}

template <typename T> buffer2D<T>& operator-=(buffer2D<T>& a, buffer2D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data += b.data;
}
template <typename T> buffer2D<T>& operator-=(buffer2D<T>& a, T const& b)
{
    a.data -= b;
}
template <typename T> buffer2D<T>  operator-(buffer2D<T> const& a, buffer2D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    buffer2D<T> res(a.dimension);
    res.data = a.data-b.data;
    return res;
}
template <typename T> buffer2D<T>  operator-(buffer2D<T> const& a, T const& b)
{
    buffer2D<T> res(a.dimension);
    res.data = a.data-b;
    return res;
}
template <typename T> buffer2D<T>  operator-(T const& a, buffer2D<T> const& b)
{
    buffer2D<T> res(a.dimension);
    res.data = a-b.data;
    return res;
}

template <typename T> buffer2D<T>& operator*=(buffer2D<T>& a, buffer2D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data *= b.data;
}
template <typename T> buffer2D<T>& operator*=(buffer2D<T>& a, float b)
{
    a.data *= b;
}
template <typename T> buffer2D<T>  operator*(buffer2D<T> const& a, buffer2D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    buffer2D<T> res(a.dimension);
    res.data = a.data*b.data;
    return res;
}
template <typename T> buffer2D<T>  operator*(buffer2D<T> const& a, float b)
{
    buffer2D<T> res(a.dimension);
    res.data = a.data*b;
    return res;
}
template <typename T> buffer2D<T>  operator*(float a, buffer2D<T> const& b)
{
    buffer2D<T> res(b.dimension);
    res.data = a*b.data;
    return res;
}

template <typename T> buffer2D<T>& operator/=(buffer2D<T>& a, buffer2D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data /= b.data;
}
template <typename T> buffer2D<T>& operator/=(buffer2D<T>& a, float b)
{
    a.data *= b;
}
template <typename T> buffer2D<T>  operator/(buffer2D<T> const& a, buffer2D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    buffer2D<T> res(a.dimension);
    res.data = a.data/b.data;
    return res;
}
template <typename T> buffer2D<T>  operator/(buffer2D<T> const& a, float b)
{
    buffer2D<T> res(a.dimension);
    res.data = a.data/b;
    return res;
}


template <typename T>
buffer2D<T> buffer2D_from_vector(buffer<T> const& arg, size_t size_1, size_t size_2)
{
    assert_vcl(arg.size()==size_1*size_2, "Incoherent size to generate buffer2D");

    buffer2D<T> b(size_1, size_2);
    b.data = arg;

    return b;
}


}
