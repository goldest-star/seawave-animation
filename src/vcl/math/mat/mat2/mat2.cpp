#include "mat2.hpp"

#include <iostream>

namespace vcl {



mat2::mat()
    :xx(),xy(),
      yx(),yy()
{}
mat2::mat(float xx_arg,float xy_arg,
          float yx_arg,float yy_arg)
    :xx(xx_arg),xy(xy_arg),
      yx(yx_arg),yy(yy_arg)
{}

mat2 mat2::identity()
{
    return mat2(1,0,
                0,1);
}


vec2 mat2::row(std::size_t offset) const
{
    switch(offset) {
    case 0:
        return vec2{xx,xy};
    case 1:
        return vec2{yx,yy};
    default:
        std::cerr<<"Error: Try to access mat2.row("<<offset<<")"<<std::endl;
        assert(false);
    }
	abort();
}
vec2 mat2::col(std::size_t offset) const
{
    switch(offset) {
    case 0:
        return {xx,yx};
    case 1:
        return {xy,yy};
    default:
        std::cerr<<"Error: Try to access mat2.col("<<offset<<")"<<std::endl;
        assert(false);
    }
	abort();
}
mat2& mat2::set_row(std::size_t offset, const vec2& v)
{
    switch(offset) {
    case 0:
        xx=v.x; xy=v.y; break;
    case 1:
        yx=v.x; yy=v.y; break;
    default:
        std::cerr<<"Error: Try to set mat2.row("<<offset<<")"<<std::endl;
        assert(false);
    }
    return *this;
}
mat2& mat2::set_col(std::size_t offset, const vec2& v)
{
    switch(offset) {
    case 0:
        xx=v.x; yx=v.y; break;
    case 1:
        xy=v.x; yy=v.y; break;
    default:
        std::cerr<<"Error: Try to set mat2.col("<<offset<<")"<<std::endl;
        assert(false);
    }
    return *this;
}

const float& mat2::operator[](std::size_t offset) const
{
    switch(offset) {
    case 0: return xx;
    case 1: return xy;
    case 2: return yx;
    case 3: return yy;
    default:
        std::cerr<<"Error: Try to access mat2["<<offset<<"]"<<std::endl;
        assert(false);
    }
	abort();
}
float& mat2::operator[](std::size_t offset)
{
    switch(offset) {
    case 0: return xx;
    case 1: return xy;
    case 2: return yx;
    case 3: return yy;
    default:
        std::cerr<<"Error: Try to access mat2["<<offset<<"]"<<std::endl;
        assert(false);
    }

	abort();
}

const float& mat2::operator()(std::size_t index1, std::size_t index2) const
{
    switch(index1) {
    case 0:
        switch(index2) {
        case 0: return xx;
        case 1: return xy;
        default:
            std::cerr<<"Error: Try to access mat2("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 1:
        switch(index2) {
        case 0: return yx;
        case 1: return yy;
        default:
            std::cerr<<"Error: Try to access mat2("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    default:
        std::cerr<<"Error: Try to access mat2("<<index1<<","<<index2<<")"<<std::endl;
        assert(false);
    }
	abort();
}
float& mat2::operator()(std::size_t index1, std::size_t index2)
{
    switch(index1) {
    case 0:
        switch(index2) {
        case 0: return xx;
        case 1: return xy;
        default:
            std::cerr<<"Error: Try to access mat2("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 1:
        switch(index2) {
        case 0: return yx;
        case 1: return yy;
        default:
            std::cerr<<"Error: Try to access mat2("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    default:
        std::cerr<<"Error: Try to access mat2("<<index1<<","<<index2<<")"<<std::endl;
        assert(false);
    }
	abort();
}


float det(const mat2& m)
{
    return m.xx*m.yy - m.xy*m.yx;
}
mat2 inverse(const mat2& m)
{
    const float d = det(m);
    assert( std::abs(d)>1e-5f );

    return mat2(  m.yy,-m.xy,
                 -m.yx, m.xx)/d;
}

}
