
#pragma once

#include "../../vec/vec.hpp"


#include <array>
#include <cassert>



namespace vcl {

/** \brief Matrix class following GLSL notations
 *
 * mat are small fixed-size matrix with floats elements stored in std::array
 * mat<N1,N2> are expected to work seamlessly with vec<N>
 * Specialized version of mat2, mat3, mat4 are provided to match GLSL conventions.
 *
 * Template parameters:
 * - N1: Number of rows
 * - N2: Number of columns
 *
 * \ingroup math
 */

template <std::size_t N1, std::size_t N2>
struct mat
{
    /** Internal data stored as std::array */
    std::array<float,N1*N2> data;

    /** Generate an identity matrix. Or a diagonal of one in case of non-squared matrix.*/
    static mat identity();

    std::size_t size() const;  /**< Total number of elements N1*N2 */
    std::size_t size1() const; /**< Total number of row */
    std::size_t size2() const; /**< Total number of columns */


    vec<N1> row(std::size_t offset) const; /**< Acces to a given row (copy to vec<N1>)*/
    vec<N2> col(std::size_t offset) const; /**< Acces to a given column (copy to vec<N2>)*/
    mat<N1,N2>& set_row(std::size_t offset, const vec<N1>& v); /**< Set a given row */
    mat<N1,N2>& set_col(std::size_t offset, const vec<N2>& v); /**< Set a given column */

    /** \name Element access
     * \brief  Allows buffer(x,y) access, or direct 1D-offset-indexing buffer[i].
     * Bound checking is performed unless VCL_NO_DEBUG is defined. */
    ///@{
    const float& operator()(std::size_t index1, std::size_t index2) const;
    float& operator()(std::size_t index1, std::size_t index2);

    const float& operator[](std::size_t offset) const;
    float& operator[](std::size_t offset);
    ///@}

};




/** \name Math operators
 * \brief Common mathematical operations between mat and vector.
*/
///@{

/** Matrix multiplication. \relates mat \ingroup math */
template <std::size_t N1, std::size_t N2, std::size_t N3> mat<N1,N3> operator*(const mat<N1,N2>& a, const mat<N2,N3>& b);
/** Matrix vector multiplication. \relates mat */
template <std::size_t N1, std::size_t N2> vec<N1> operator*(const mat<N1,N2>& a, const vec<N2>& b);


/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2>& operator+=(mat<N1,N2>& a, const mat<N1,N2>& b);
/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2>& operator-=(mat<N1,N2>& a, const mat<N1,N2>& b);
/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2>& operator*=(mat<N1,N2>& a, float b);
/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2>& operator/=(mat<N1,N2>& a, const float b);

/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2> operator+(const mat<N1,N2>& a, const mat<N1,N2>& b);
/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2> operator-(const mat<N1,N2>& a, const mat<N1,N2>& b);
/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2> operator*(const mat<N1,N2>& a, float b);
/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2> operator*(float a, const mat<N1,N2>& b);

/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2> operator-(const mat<N1,N2>& a);

/** \relates mat */
template <std::size_t N1, std::size_t N2> mat<N1,N2> operator/(const mat<N1,N2>& a, const float b);
///@}


/** Matrix transpose. \relates mat \ingroup math */
template <std::size_t N> mat<N,N> transpose(const mat<N,N>& m);


/** \name Display mat
 * \brief Ways to display a mat, or convert it to string. */
///@{
/** Convert a mat to string, displayable as a 2D structure. \relates mat \ingroup math */
template <std::size_t N1, std::size_t N2> std::string to_string_mat(const mat<N1,N2>& v, const std::string& element_separator=", ", const std::string& line_separator="\n", const std::string& line_start="(", const std::string& line_end=")");
/** Convert a mat to string as a 1D structure. \relates mat */
template <std::size_t N1, std::size_t N2> std::string to_string(const mat<N1,N2>& v, const std::string& separator=" ");
/** Default matrix display on the terminal. \relates mat */
template <std::size_t N1, std::size_t N2> std::ostream& operator<<(std::ostream& s, const mat<N1,N2>& v);
///@}

/** Check (approximate) equality between two mat. \relates mat \ingroup math */
template <std::size_t N1, std::size_t N2> bool is_equal(const mat<N1,N2>& a, const mat<N1,N2>& b);



}





/* ************************************************** */
/*           IMPLEMENTATION                           */
/* ************************************************** */

namespace vcl{



template <std::size_t N1, std::size_t N2>
vec<N1> mat<N1,N2>::row(std::size_t offset) const
{
    assert( offset<N1 );

    vec<N1> out;
    for( std::size_t k=0; k<N1; ++k)
        out[k] = (*this)(k,offset);
    return out;
}

template <std::size_t N1, std::size_t N2>
vec<N2> mat<N1,N2>::col(std::size_t offset) const
{
    assert( offset<N2 );

    vec<N2> out;
    for( std::size_t k=0; k<N2; ++k)
        out[k] = (*this)(offset,k);
    return out;
}


template <std::size_t N1, std::size_t N2>
mat<N1,N2>& mat<N1,N2>::set_row(std::size_t offset, const vec<N1>& v)
{
    assert( offset<N1 );
    for( std::size_t k=0; k<N1; ++k)
        (*this)(k,offset) = v[k];
    return *this;
}

template <std::size_t N1, std::size_t N2>
mat<N1,N2>& mat<N1,N2>::set_col(std::size_t offset, const vec<N2>& v)
{
    assert( offset<N2 );
    for( std::size_t k=0; k<N2; ++k)
        (*this)(offset,k) = v[k];
    return *this;
}



template <std::size_t N1, std::size_t N2> std::size_t mat<N1,N2>::size() const
{
    return N1*N2;
}
template <std::size_t N1, std::size_t N2>
const float& mat<N1,N2>::operator[](std::size_t offset) const
{
    assert(offset<N1*N2);
    return data[offset];
}
template <std::size_t N1, std::size_t N2> float& mat<N1,N2>::operator[](std::size_t offset)
{
    assert(offset<N1*N2);
    return data[offset];
}


template <std::size_t N1, std::size_t N2> mat<N1,N2> mat<N1,N2>::identity()
{
    mat<N1,N2> M;
    std::size_t Nm = std::min(N1,N2);

    for(std::size_t k=0; k<Nm; ++k) {
        M(k,k) = 1.0f;
    }

    return M;
}

template <std::size_t N1, std::size_t N2>
const float& mat<N1,N2>::operator()(std::size_t index1, std::size_t index2) const
{
    assert(index1<N1);
    assert(index2<N2);

    std::size_t offset = index2 + N2*index1;
    return (*this)[offset];

}
template <std::size_t N1, std::size_t N2> float& mat<N1,N2>::operator()(std::size_t index1, std::size_t index2)
{
    assert(index1<N1);
    assert(index2<N2);

    std::size_t offset = index2 + N2*index1;
    return (*this)[offset];
}

template <std::size_t N1, std::size_t N2, std::size_t N3> mat<N1,N3> operator*(const mat<N1,N2>& a, const mat<N2,N3>& b)
{
    mat<N1,N3> c;
    for(std::size_t k1=0; k1<N1; ++k1) {
        for(std::size_t k3=0; k3<N3; ++k3) {
            float value = 0.0f;
            for(std::size_t k2=0; k2<N2; ++k2) {
                value += a(k1,k2)*b(k2,k3);
            }
            c(k1,k3) = value;
        }
    }

    return c;
}

template <std::size_t N1, std::size_t N2> vec<N1> operator*(const mat<N1,N2>& a, const vec<N2>& b)
{
    vec<N1> c;
    for(std::size_t k1=0; k1<N1; ++k1) {

        float value = 0.0f;
        for(std::size_t k2=0; k2<N2; ++k2) {
            value += a(k1,k2)*b[k2];
        }
        c[k1] = value;
    }
    return c;
}



template <std::size_t N1, std::size_t N2> mat<N1,N2>& operator+=(mat<N1,N2>& a, const mat<N1,N2>& b)
{
    constexpr std::size_t N = N1*N2;
    for(size_t k=0; k<N; ++k)
        a[k] += b[k];
    return a;
}
template <std::size_t N1, std::size_t N2> mat<N1,N2>& operator-=(mat<N1,N2>& a, const mat<N1,N2>& b)
{
    constexpr std::size_t N = N1*N2;
    for(size_t k=0; k<N; ++k)
        a[k] -= b[k];
    return a;
}
template <std::size_t N1, std::size_t N2>
mat<N1,N2>& operator/=(mat<N1,N2>& a, const float b)
{
    constexpr std::size_t N = N1*N2;
    for(size_t k=0; k<N; ++k)
        a[k] /= b;
    return a;
}

template <std::size_t N1, std::size_t N2> mat<N1,N2> operator+(const mat<N1,N2>& a, const mat<N1,N2>& b)
{
    auto c = a;
    c += b;
    return c;
}

template <std::size_t N1, std::size_t N2> mat<N1,N2> operator-(const mat<N1,N2>& a, const mat<N1,N2>& b)
{
    auto c=a;
    c -= b;
    return c;
}

template <std::size_t N1, std::size_t N2> mat<N1,N2>& operator*=(mat<N1,N2>& a, float b)
{
    constexpr std::size_t N = N1*N2;
    for(size_t k=0; k<N; ++k)
        a[k] *= b;
    return a;
}
template <std::size_t N1, std::size_t N2> mat<N1,N2> operator*(const mat<N1,N2>& a, float b)
{
    auto c=a;
    c *= b;
    return c;
}
template <std::size_t N1, std::size_t N2> mat<N1,N2> operator*(float a, const mat<N1,N2>& b)
{
    return b*a;
}

template <std::size_t N1, std::size_t N2> mat<N1,N2> operator-(const mat<N1,N2>& a)
{
    mat<N1,N2> b;
    constexpr std::size_t N = N1*N2;
    for(size_t k=0; k<N; ++k)
        b[k] = -a[k];
    return b;
}

template <std::size_t N1, std::size_t N2>
mat<N1,N2> operator/(const mat<N1,N2>& a, const float b)
{
    auto c = a;
    c /= b;
    return c;
}

template <std::size_t N1, std::size_t N2> std::string to_string_mat(const mat<N1,N2>& v, const std::string& element_separator, const std::string& line_separator, const std::string& line_start, const std::string& line_end)
{
    using std::to_string;

    std::string s;
    for(std::size_t k1=0; k1<N1; ++k1) {

        s += line_start;
        for(std::size_t k2=0; k2<N2; ++k2) {

            s += to_string(v(k1,k2));
            if( k2<N2-1 )
                s += element_separator;

        }
        s += line_end;
        if( k1<N1-1 )
            s += line_separator;
    }

    return s;
}

template <std::size_t N1, std::size_t N2> std::string to_string(const mat<N1,N2>& v, const std::string& separator)
{
    std::string s;
    constexpr std::size_t N = N1*N2;

    if( N==0 )
        return "";

    for(std::size_t k=0; k<N-1; ++k) {
        s += std::to_string(v[k])+separator;
    }
    s += std::to_string(v[N-1]);

    return s;
}
template <std::size_t N1, std::size_t N2> std::ostream& operator<<(std::ostream& s, const mat<N1,N2>& v)
{
    s << to_string(v);
    return s;
}

template <std::size_t N1, std::size_t N2> bool is_equal(const mat<N1,N2>& a, const mat<N1,N2>& b)
{
    constexpr std::size_t N = N1*N2;
    for(std::size_t k=0; k<N; ++k)
        if( !is_equal(a[k],b[k]) )
            return false;
    return true;
}

template <std::size_t N>
mat<N,N> transpose(const mat<N,N>& m)
{
    mat<N,N> out;
    for(std::size_t k1=0; k1<N; ++k1)
        for(std::size_t k2=0; k2<N; ++k2)
            out(k1,k2) = m(k2,k1);
    return out;
}

template <std::size_t Nsub_1, std::size_t Nsub_2,std::size_t N1, std::size_t N2>
mat<Nsub_1,Nsub_2> submat(const mat<N1,N2>& m, std::size_t offset_1, std::size_t offset_2)
{
    static_assert(Nsub_1<=N1,"Sub-matrix should have size <= size of the original matrix");
    static_assert(Nsub_2<=N2,"Sub-matrix should have size <= size of the original matrix");

    assert(offset_1+Nsub_1<=N1);
    assert(offset_2+Nsub_2<=N2);
//    if( offset_1+Nsub_1>=N1  || offset_2+Nsub_2>=N2 ) {
//        std::cerr<<" Error, try to extract submat ["<<offset_1<<":"<<offset_1+N1<<" , "<<offset_2<<":"<<offset_2+N2<<"] on mattrix of size "<<N1<<"x"<<N2<<std::endl;
//        abort();
//    }

    mat<Nsub_1,Nsub_2> m_out;
    for( size_t k1=0; k1<Nsub_1; ++k1) {
        for( size_t k2=0; k2<Nsub_2; ++k2) {
            m_out(k1,k2) = m(k1+offset_1,k2+offset_2);
        }
    }
    return m_out;
}

template <std::size_t Nsub_1, std::size_t Nsub_2,std::size_t N1, std::size_t N2>
void set_submat(mat<N1,N2>& m_out, std::size_t offset_1, std::size_t offset_2,const mat<Nsub_1,Nsub_2>& m_in)
{
    static_assert(Nsub_1<=N1,"Sub-matrix should have size <= size of the original matrix");
    static_assert(Nsub_2<=N2,"Sub-matrix should have size <= size of the original matrix");

    assert(offset_1+Nsub_1<=N1);
    assert(offset_2+Nsub_2<=N2);
    /*
    if( offset_1+Nsub_1>=N1  || offset_2+Nsub_2>=N2 ) {
        std::cerr<<" Error, try to set submat ["<<offset_1<<":"<<offset_1+N1<<" , "<<offset_2<<":"<<offset_2+N2<<"] on mattrix of size "<<N1<<"x"<<N2<<std::endl;
        abort();
    }*/

    for( size_t k1=0; k1<Nsub_1; ++k1) {
        for( size_t k2=0; k2<Nsub_2; ++k2) {
            m_out(k1+offset_1,k2+offset_2) = m_in(k1,k2);
        }
    }
}

template <std::size_t N>
vec<N> mat2vec(const mat<1,N>& m)
{
    vec<N> v;
    for( size_t k=0; k<N; ++k) {
        v[k] = m[k];
    }
    return v;
}

template <std::size_t N>
vec<N> mat2vec(const mat<N,1>& m)
{
    vec<N> v;
    for( size_t k=0; k<N; ++k) {
        v[k] = m[k];
    }
    return v;
}

template <std::size_t N>
mat<N,1> vec2mat(const vec<N>& v)
{
    mat<N,1> m;
    for( size_t k=0; k<N; ++k) {
        m[k] = v[k];
    }
    return m;
}


}
