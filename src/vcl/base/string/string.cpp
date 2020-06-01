#include "string.hpp"

namespace vcl
{

std::string zero_fill(const std::string& input, size_t number_of_zero)
{
    std::string new_string = "";
    const size_t N = input.size();
    for(size_t k=N; k<number_of_zero; ++k)
        new_string += "0";
    new_string += input;
    return new_string;
}

std::string to_string(std::string const& s)
{
    return s;
}

std::string to_string(char const* s)
{
    return std::string(s);
}

bool is_equal(std::string const& a, std::string const& b)
{
    return a==b;
}


}
