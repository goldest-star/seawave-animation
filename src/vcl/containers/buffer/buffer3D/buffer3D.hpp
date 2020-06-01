#pragma once

#include "../buffer/buffer.hpp"

#include "vcl/base/base.hpp"


/* ************************************************** */
/*           Header                                   */
/* ************************************************** */

namespace vcl
{

template <typename T>
struct buffer3D
{
    size_t3 dimension;
    buffer<T> data;

    buffer3D();
    buffer3D(size_t size);
    buffer3D(size_t3 const& size);
    buffer3D(size_t size_1, size_t size_2, size_t size_3);

    void clear();
    size_t size() const;
    void resize(size_t size); // Create a square buffer3D of dimension NxNXN
    void resize(size_t3 const& size);
    void resize(size_t size_1, size_t size_2, size_t size_3);
    void fill(T const& value);

    T const& operator[](size_t const& index) const;
    T & operator[](size_t const& index);
    T const& operator()(size_t const& index) const;
    T & operator()(size_t const& index);

    T const& operator[](size_t3 const& index) const;
    T & operator[](size_t3 const& index);
    T const& operator()(size_t3 const& index) const;
    T & operator()(size_t3 const& index);
    T const& operator()(size_t k1, size_t k2, size_t k3) const;
    T & operator()(size_t k1, size_t k2, size_t k3);

    typename std::vector<T>::iterator begin();
    typename std::vector<T>::iterator end();
    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::const_iterator end() const;
    typename std::vector<T>::const_iterator cbegin() const;
    typename std::vector<T>::const_iterator cend() const;
};

template <typename T> std::ostream& operator<<(std::ostream& s, buffer3D<T> const& v);
template <typename T> std::string to_string(buffer3D<T> const& v, std::string const& separator=" ");

template <typename T> buffer3D<T>& operator+=(buffer3D<T>& a, buffer3D<T> const& b);
template <typename T> buffer3D<T>& operator+=(buffer3D<T>& a, T const& b);
template <typename T> buffer3D<T>  operator+(buffer3D<T> const& a, buffer3D<T> const& b);
template <typename T> buffer3D<T>  operator+(buffer3D<T> const& a, T const& b);
template <typename T> buffer3D<T>  operator+(T const& a, buffer3D<T> const& b);

template <typename T> buffer3D<T>& operator-=(buffer3D<T>& a, buffer3D<T> const& b);
template <typename T> buffer3D<T>& operator-=(buffer3D<T>& a, T const& b);
template <typename T> buffer3D<T>  operator-(buffer3D<T> const& a, buffer3D<T> const& b);
template <typename T> buffer3D<T>  operator-(buffer3D<T> const& a, T const& b);
template <typename T> buffer3D<T>  operator-(T const& a, buffer3D<T> const& b);

template <typename T> buffer3D<T>& operator*=(buffer3D<T>& a, buffer3D<T> const& b);
template <typename T> buffer3D<T>& operator*=(buffer3D<T>& a, float b);
template <typename T> buffer3D<T>  operator*(buffer3D<T> const& a, buffer3D<T> const& b);
template <typename T> buffer3D<T>  operator*(buffer3D<T> const& a, float b);
template <typename T> buffer3D<T>  operator*(float a, buffer3D<T> const& b);

template <typename T> buffer3D<T>& operator/=(buffer3D<T>& a, buffer3D<T> const& b);
template <typename T> buffer3D<T>& operator/=(buffer3D<T>& a, float b);
template <typename T> buffer3D<T>  operator/(buffer3D<T> const& a, buffer3D<T> const& b);
template <typename T> buffer3D<T>  operator/(buffer3D<T> const& a, float b);
template <typename T> buffer3D<T>  operator/(float a, buffer3D<T> const& b);

}



/* ************************************************** */
/*           IMPLEMENTATION                           */
/* ************************************************** */

namespace vcl
{

static inline size_t offset(size_t3 const& k, size_t3 const& dim)
{
    return k[0]+dim[0]*(k[1]+dim[1]*k[2]);
}


template <typename T>
buffer3D<T>::buffer3D()
    :dimension(size_t3{0,0,0}),data()
{}

template <typename T>
buffer3D<T>::buffer3D(size_t size)
    :dimension({size,size,size}),data(size*size*size)
{}

template <typename T>
buffer3D<T>::buffer3D(size_t3 const& size)
    :dimension(size),data(size[0]*size[1]*size[2])
{}

template <typename T>
buffer3D<T>::buffer3D(size_t size_1, size_t size_2, size_t size_3)
    :dimension({size_1,size_2}),data(size_1*size_2*size_3)
{}

template <typename T>
size_t buffer3D<T>::size() const
{
    return dimension[0]*dimension[1]*dimension[2];
}

template <typename T>
void buffer3D<T>::resize(size_t size)
{
    resize(size,size,size);
}

template <typename T>
void buffer3D<T>::resize(size_t3 const& size)
{
    dimension = size;
    data.resize(size[0]*size[1]*size[2]);
}

template <typename T>
void buffer3D<T>::resize(size_t size_1, size_t size_2, size_t size_3)
{
    dimension = {size_1, size_2, size_3};
    resize({size_1, size_2, size_3});
}

template <typename T>
void buffer3D<T>::fill(T const& value)
{
    data.fill(value);
}


template <typename T>
T const& buffer3D<T>::operator[](size_t const& index) const
{
    assert_vcl(index<data.size(), "Index="+str(index));
    return data[index];
}

template <typename T>
T & buffer3D<T>::operator[](size_t const& index)
{
    assert_vcl(index<data.size(), "Index="+str(index));
    return data[index];
}

template <typename T>
T const& buffer3D<T>::operator()(size_t const& index) const
{
    return (*this)[index];
}

template <typename T>
T & buffer3D<T>::operator()(size_t const& index)
{
    return (*this)[index];
}



template <typename T>
T const& buffer3D<T>::operator[](size_t3 const& index) const
{
    assert_vcl(index[0]<dimension[0], "index=("+str(index)+"), dimension=("+str(dimension)+")");
    assert_vcl(index[1]<dimension[1], "index=("+str(index)+"), dimension=("+str(dimension)+")");
    assert_vcl(index[2]<dimension[2], "index=("+str(index)+"), dimension=("+str(dimension)+")");

    size_t const k = offset(index,dimension);
    assert_vcl(k<data.size(), "Should never happen");

    return data[k];
}

template <typename T>
T & buffer3D<T>::operator[](size_t3 const& index)
{
    assert_vcl(index[0]<dimension[0], "index=("+str(index)+"), dimension=("+str(dimension)+")");
    assert_vcl(index[1]<dimension[1], "index=("+str(index)+"), dimension=("+str(dimension)+")");
    assert_vcl(index[2]<dimension[2], "index=("+str(index)+"), dimension=("+str(dimension)+")");

    size_t const k = offset(index,dimension);
    assert_vcl(k<data.size(), "Should never happen");

    return data[k];
}



template <typename T>
T const& buffer3D<T>::operator()(size_t3 const& index) const
{
    return (*this)[index];
}

template <typename T>
T & buffer3D<T>::operator()(size_t3 const& index)
{
    return (*this)[index];
}

template <typename T>
T const& buffer3D<T>::operator()(size_t k1, size_t k2, size_t k3) const
{
    return (*this)({k1,k2,k3});
}

template <typename T>
T & buffer3D<T>::operator()(size_t k1, size_t k2, size_t k3)
{
    return (*this)({k1,k2,k3});
}

template <typename T>
typename std::vector<T>::iterator buffer3D<T>::begin()
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::iterator buffer3D<T>::end()
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator buffer3D<T>::begin() const
{
    return data.begin();
}

template <typename T>
typename std::vector<T>::const_iterator buffer3D<T>::end() const
{
    return data.end();
}

template <typename T>
typename std::vector<T>::const_iterator buffer3D<T>::cbegin() const
{
    return data.cbegin();
}

template <typename T>
typename std::vector<T>::const_iterator buffer3D<T>::cend() const
{
    return data.cend();
}

template <typename T> std::ostream& operator<<(std::ostream& s, buffer3D<T> const& v)
{
    return s << v.data;
}
template <typename T> std::string to_string(buffer3D<T> const& v, std::string const& separator)
{
    return to_string(v.data, separator);
}


template <typename T> buffer3D<T>& operator+=(buffer3D<T>& a, buffer3D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data += b.data;
}
template <typename T> buffer3D<T>& operator+=(buffer3D<T>& a, T const& b)
{
    a.data += b;
}
template <typename T> buffer3D<T>  operator+(buffer3D<T> const& a, buffer3D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    buffer3D<T> res(a.dimension);
    res.data = a.data+b.data;
    return res;

}
template <typename T> buffer3D<T>  operator+(buffer3D<T> const& a, T const& b)
{
    buffer3D<T> res(a.dimension);
    res.data = a.data+b;
    return res;
}
template <typename T> buffer3D<T>  operator+(T const& a, buffer3D<T> const& b)
{
    buffer3D<T> res(b.dimension);
    res.data = a + b.data;
    return res;
}

template <typename T> buffer3D<T>& operator-=(buffer3D<T>& a, buffer3D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data += b.data;
}
template <typename T> buffer3D<T>& operator-=(buffer3D<T>& a, T const& b)
{
    a.data -= b;
}
template <typename T> buffer3D<T>  operator-(buffer3D<T> const& a, buffer3D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    buffer3D<T> res(a.dimension);
    res.data = a.data-b.data;
    return res;
}
template <typename T> buffer3D<T>  operator-(buffer3D<T> const& a, T const& b)
{
    buffer3D<T> res(a.dimension);
    res.data = a.data-b;
    return res;
}
template <typename T> buffer3D<T>  operator-(T const& a, buffer3D<T> const& b)
{
    buffer3D<T> res(a.dimension);
    res.data = a-b.data;
    return res;
}

template <typename T> buffer3D<T>& operator*=(buffer3D<T>& a, buffer3D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data *= b.data;
}
template <typename T> buffer3D<T>& operator*=(buffer3D<T>& a, float b)
{
    a.data *= b;
}
template <typename T> buffer3D<T>  operator*(buffer3D<T> const& a, buffer3D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    buffer3D<T> res(a.dimension);
    res.data = a.data*b.data;
    return res;
}
template <typename T> buffer3D<T>  operator*(buffer3D<T> const& a, float b)
{
    buffer3D<T> res(a.dimension);
    res.data = a.data*b;
    return res;
}
template <typename T> buffer3D<T>  operator*(float a, buffer3D<T> const& b)
{
    buffer3D<T> res(b.dimension);
    res.data = a*b.data;
    return res;
}

template <typename T> buffer3D<T>& operator/=(buffer3D<T>& a, buffer3D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    a.data /= b.data;
}
template <typename T> buffer3D<T>& operator/=(buffer3D<T>& a, float b)
{
    a.data *= b;
}
template <typename T> buffer3D<T>  operator/(buffer3D<T> const& a, buffer3D<T> const& b)
{
    assert_vcl( is_equal(a.dimension,b.dimension), "Dimension do not agree: a:"+str(a.dimension)+", b:"+str(b.dimension) );
    buffer3D<T> res(a.dimension);
    res.data = a.data/b.data;
    return res;
}
template <typename T> buffer3D<T>  operator/(buffer3D<T> const& a, float b)
{
    buffer3D<T> res(a.dimension);
    res.data = a.data/b;
    return res;
}
template <typename T> buffer3D<T>  operator/(float a, buffer3D<T> const& b)
{
    buffer3D<T> res(b.dimension);
    res.data = a/b.data;
    return res;
}



}
