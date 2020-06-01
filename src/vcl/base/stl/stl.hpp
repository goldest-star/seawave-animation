#pragma once

#include <array>

namespace vcl
{

/** Check (possibly approximate) equality between 2 arrays
 * \ingroup container */
template <typename T1, typename T2, size_t N>
bool is_equal( std::array<T1,N> const& a, std::array<T2,N> const& b );

}

// Template implementation

namespace vcl
{

template <typename T1, typename T2, size_t N>
bool is_equal( std::array<T1,N> const& a, std::array<T2,N> const& b )
{
    for(size_t k=0; k<N; ++k)
        if( is_equal(a[k],b[k])==false )
            return false;
    return true;
}

}
