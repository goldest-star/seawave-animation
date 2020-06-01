#pragma once

#include <string>
#include <vector>
#include <array>

#include "../basic_types/basic_types.hpp"

// Helper functions with respect to string handling and conversion

namespace vcl
{

/** Convert a generic type to a string */
template <typename T> std::string str(const T& x);
/** Convert a generic type to a string */
template <typename T> std::string to_string(const T& x);

/** add zeros in from of the string */
std::string zero_fill(const std::string& input, size_t number_of_zero);

/** Export content of std::array into a string
 * \ingroup container
*/
template <typename T, size_t N>
std::string to_string(std::array<T,N> v, const std::string& separator=" ");

/** Export content of std::vector into a string
 * \ingroup container
*/
template <typename T>
std::string to_string(std::vector<T> v, const std::string& separator=" ");

/** Allows to call to_string on itself */
std::string to_string(std::string const& s);
/** Allows to call to_string a C-string */
std::string to_string(char const* s);

/** Check equality between string (using ==) */
bool is_equal(std::string const& a, std::string const& b);

namespace detail{

/** Generic template function to export string from a container */
template <typename T>
std::string to_string_container(T const& v, const std::string& separator=" ");
}


// Template implementation





template <typename T> std::string str(const T& x)
{
    using vcl::to_string;
    return to_string(x);
}

template <typename T> std::string to_string(const T& x)
{
    return std::to_string(x);
}

template <typename T, size_t N>
std::string to_string(std::array<T,N> v, const std::string& separator)
{
    return detail::to_string_container(v, separator);
}

template <typename T>
std::string to_string(std::vector<T> v, const std::string& separator)
{
    return detail::to_string_container(v, separator);
}


namespace detail{
template <typename T> std::string to_string_container(T const& v, const std::string& separator)
{
    if( v.size()==0 )
        return "";

    std::string s;

    auto it = v.cbegin();
    auto it_next = it; ++it_next;
    auto const it_end = v.cend();

    using vcl::to_string;
    while( it!=it_end )
    {
        s += to_string(*it);
        if(it_next!=it_end)
            s += separator;

        ++it;
        ++it_next;
    }

    return s;
}
}



}
