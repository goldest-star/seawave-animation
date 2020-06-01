#include "mat3.hpp"

#include <iostream>

namespace vcl {



mat3::mat()
    :xx(1),xy(0),xz(0),yx(0),yy(1),yz(0),zx(0),zy(0),zz(1)
{}
mat3::mat(float xx_arg,float xy_arg,float xz_arg,
          float yx_arg,float yy_arg,float yz_arg,
          float zx_arg,float zy_arg,float zz_arg)
    :xx(xx_arg),xy(xy_arg),xz(xz_arg),
      yx(yx_arg),yy(yy_arg),yz(yz_arg),
      zx(zx_arg),zy(zy_arg),zz(zz_arg)
{}
mat3::mat(vec3 const& column0, vec3 const& column1, vec3 const& column2)
    :xx(column0.x),xy(column1.x),xz(column2.x),
      yx(column0.y),yy(column1.y),yz(column2.y),
      zx(column0.z),zy(column1.z),zz(column2.z)
{}

mat3 mat3::identity()
{
    return mat3(1,0,0,
                0,1,0,
                0,0,1);
}

mat3 mat3::zero()
{
    return mat3(0,0,0,
                0,0,0,
                0,0,0);
}


mat3 mat3::from_scaling(float s)
{
    return mat3(s,0,0,
                0,s,0,
                0,0,s);
}

mat3 mat3::from_scaling(const vcl::vec3& s)
{
    return mat3(s.x,0,0,
                0,s.y,0,
                0,0,s.z);
}


vec3 mat3::row(std::size_t offset) const
{
    switch(offset) {
    case 0:
        return {xx,xy,xz};
    case 1:
        return {yx,yy,yz};
    case 2:
        return {zx,zy,zz};
    default:
        std::cerr<<"Error: Try to access mat3.row("<<offset<<")"<<std::endl;
        assert(false);
    }
	abort();
}
vec3 mat3::col(std::size_t offset) const
{
    switch(offset) {
    case 0:
        return {xx,yx,zx};
    case 1:
        return {xy,yy,zy};
    case 2:
        return {xz,yz,zz};
    default:
        std::cerr<<"Error: Try to access mat3.col("<<offset<<")"<<std::endl;
        assert(false);
    }
	abort();
}
mat3& mat3::set_row(std::size_t offset, const vec3& v)
{
    switch(offset) {
    case 0:
        xx=v.x; xy=v.y; xz=v.z; break;
    case 1:
        yx=v.x; yy=v.y; yz=v.z; break;
    case 2:
        zx=v.x; zy=v.y; zz=v.z; break;
    default:
        std::cerr<<"Error: Try to set mat3.row("<<offset<<")"<<std::endl;
        assert(false);
    }
    return *this;
}
mat3& mat3::set_col(std::size_t offset, const vec3& v)
{
    switch(offset) {
    case 0:
        xx=v.x; yx=v.y; zx=v.z; break;
    case 1:
        xy=v.x; yy=v.y; zy=v.z; break;
    case 2:
        xz=v.x; yz=v.y; zz=v.z; break;
    default:
        std::cerr<<"Error: Try to set mat3.col("<<offset<<")"<<std::endl;
        assert(false);
    }
    return *this;
}

const float& mat3::operator[](std::size_t offset) const
{
    switch(offset) {
    case 0: return xx;
    case 1: return xy;
    case 2: return xz;
    case 3: return yx;
    case 4: return yy;
    case 5: return yz;
    case 6: return zx;
    case 7: return zy;
    case 8: return zz;
    default:
        std::cerr<<"Error: Try to access mat3["<<offset<<"]"<<std::endl;
        assert(false);
    }
	abort();
}
float& mat3::operator[](std::size_t offset)
{
    switch(offset) {
    case 0: return xx;
    case 1: return xy;
    case 2: return xz;
    case 3: return yx;
    case 4: return yy;
    case 5: return yz;
    case 6: return zx;
    case 7: return zy;
    case 8: return zz;
    default:
        std::cerr<<"Error: Try to access mat3["<<offset<<"]"<<std::endl;
        assert(false);
    }
	abort();
}

const float& mat3::operator()(std::size_t index1, std::size_t index2) const
{
    switch(index1) {
    case 0:
        switch(index2) {
        case 0: return xx;
        case 1: return xy;
        case 2: return xz;
        default:
            std::cerr<<"Error: Try to access mat3("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 1:
        switch(index2) {
        case 0: return yx;
        case 1: return yy;
        case 2: return yz;
        default:
            std::cerr<<"Error: Try to access mat3("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 2:
        switch(index2) {
        case 0: return zx;
        case 1: return zy;
        case 2: return zz;
        default:
            std::cerr<<"Error: Try to access mat3("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    default:
        std::cerr<<"Error: Try to access mat3("<<index1<<","<<index2<<")"<<std::endl;
        assert(false);
    }
	abort();
}
float& mat3::operator()(std::size_t index1, std::size_t index2)
{
    switch(index1) {
    case 0:
        switch(index2) {
        case 0: return xx;
        case 1: return xy;
        case 2: return xz;
        default:
            std::cerr<<"Error: Try to access mat3("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 1:
        switch(index2) {
        case 0: return yx;
        case 1: return yy;
        case 2: return yz;
        default:
            std::cerr<<"Error: Try to access mat3("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 2:
        switch(index2) {
        case 0: return zx;
        case 1: return zy;
        case 2: return zz;
        default:
            std::cerr<<"Error: Try to access mat3("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    default:
        std::cerr<<"Error: Try to access mat3("<<index1<<","<<index2<<")"<<std::endl;
        assert(false);
    }
	abort();
}


float det(const mat3& m)
{
    return m.xx*m.yy*m.zz + m.xy*m.yz*m.zx + m.yx*m.zy*m.xz
            - (m.zx*m.yy*m.xz + m.zy*m.yz*m.xx + m.yx*m.xy*m.zz);
}
mat3 inverse(const mat3& m)
{
    const float d = det(m);
    assert( std::abs(d)>1e-6f );

    const float x00 =   m[4]*m[8]-m[7]*m[5];
    const float x01 = -(m[1]*m[8]-m[7]*m[2]);
    const float x02 =   m[1]*m[5]-m[4]*m[2];


    const float x10 = -(m[3]*m[8]-m[6]*m[5]);
    const float x11 =   m[0]*m[8]-m[6]*m[2];
    const float x12 = -(m[0]*m[5]-m[3]*m[2]);

    const float x20 =   m[3]*m[7]-m[6]*m[4];
    const float x21 = -(m[0]*m[7]-m[6]*m[1]);
    const float x22 =   m[0]*m[4]-m[3]*m[1];

    return mat3(x00,x01,x02,
                x10,x11,x12,
                x20,x21,x22)/d;
}



}
