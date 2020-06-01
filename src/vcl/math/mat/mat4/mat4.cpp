#include "mat4.hpp"

#include "../../mat/mat3/mat3.hpp"
#include "../../vec/vec3/vec3.hpp"

#include <iostream>

namespace vcl {



mat4::mat()
    :xx(),xy(),xz(),xw(),
      yx(),yy(),yz(),yw(),
      zx(),zy(),zz(),zw(),
      wx(),wy(),wz(),ww()
{}
mat4::mat(float xx_arg,float xy_arg,float xz_arg,float xw_arg,
          float yx_arg,float yy_arg,float yz_arg,float yw_arg,
          float zx_arg,float zy_arg,float zz_arg,float zw_arg,
          float wx_arg,float wy_arg,float wz_arg,float ww_arg)
    :xx(xx_arg),xy(xy_arg),xz(xz_arg),xw(xw_arg),
      yx(yx_arg),yy(yy_arg),yz(yz_arg),yw(yw_arg),
      zx(zx_arg),zy(zy_arg),zz(zz_arg),zw(zw_arg),
      wx(wx_arg),wy(wy_arg),wz(wz_arg),ww(ww_arg)
{}

mat4::mat(const vcl::mat3& R, const vcl::vec3& t)
    :xx(R(0,0)),xy(R(0,1)),xz(R(0,2)),xw(t.x),
      yx(R(1,0)),yy(R(1,1)),yz(R(1,2)),yw(t.y),
      zx(R(2,0)),zy(R(2,1)),zz(R(2,2)),zw(t.z),
      wx(0),wy(0),wz(0),ww(1.0f)
{
}

mat4 mat4::identity()
{
    return mat4(1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1);
}

mat4 mat4::zero()
{
    return mat4(0,0,0,0,
                0,0,0,0,
                0,0,0,0,
                0,0,0,0);
}

mat4 mat4::perspective(float angle_of_view, float image_aspect, float z_near, float z_far)
{
    const float fy = 1/std::tan(angle_of_view/2);
    const float fx = fy/image_aspect;
    const float L = z_near-z_far;

    const float C = (z_far+z_near)/L;
    const float D = (2*z_far*z_near)/L;

    return {
        fx,0,0,0,
        0,fy,0,0,
        0,0,C,D,
        0,0,-1,0
    };
}

mat4 mat4::from_scaling(float s)
{
    return {
        s,0,0,0,
        0,s,0,0,
        0,0,s,0,
        0,0,0,1
    };
}
mat4 mat4::from_scaling(const vcl::vec3& s)
{
    return {
        s.x,0  ,0  ,0,
        0  ,s.y,0  ,0,
        0  ,0  ,s.z,0,
        0  ,0  ,0  ,1
    };
}

mat4 mat4::from_mat3(const vcl::mat3& m)
{
    return {
        m.xx, m.xy, m.xz, 0,
        m.yx, m.yy, m.yz, 0,
        m.zx, m.zy, m.zz, 0,
          0 ,  0  ,  0  , 1
    };
}
mat4 mat4::from_translation(const vcl::vec3& t)
{
    return {
        1,0,0,t.x,
        0,1,0,t.y,
        0,0,1,t.z,
        0,0,0,1
    };
}



vec4 mat4::row(std::size_t offset) const
{
    switch(offset) {
    case 0:
        return {xx,xy,xz,xw};
    case 1:
        return {yx,yy,yz,yw};
    case 2:
        return {zx,zy,zz,zw};
    case 3:
        return {wx,wy,wz,ww};
    default:
        std::cerr<<"Error: Try to access mat4.row("<<offset<<")"<<std::endl;
        assert(false);
    }
	abort();
}
vec4 mat4::col(std::size_t offset) const
{
    switch(offset) {
    case 0:
        return {xx,yx,zx,wx};
    case 1:
        return {xy,yy,zy,wy};
    case 2:
        return {xz,yz,zz,wz};
    case 3:
        return {xw,yw,zw,ww};
    default:
        std::cerr<<"Error: Try to access mat4.col("<<offset<<")"<<std::endl;
        assert(false);
    }
	abort();
}
mat4& mat4::set_row(std::size_t offset, const vec4& v)
{
    switch(offset) {
    case 0:
        xx=v.x; xy=v.y; xz=v.z; wz=v.w; break;
    case 1:
        yx=v.x; yy=v.y; yz=v.z; yw=v.w; break;
    case 2:
        zx=v.x; zy=v.y; zz=v.z; zw=v.w; break;
    case 3:
        wx=v.x; wy=v.y; wz=v.z; ww=v.w; break;
    default:
        std::cerr<<"Error: Try to set mat4.row("<<offset<<")"<<std::endl;
        assert(false);
    }
    return *this;
}
mat4& mat4::set_col(std::size_t offset, const vec4& v)
{
    switch(offset) {
    case 0:
        xx=v.x; yx=v.y; zx=v.z; wx=v.w; break;
    case 1:
        xy=v.x; yy=v.y; zy=v.z; wy=v.w; break;
    case 2:
        xz=v.x; yz=v.y; zz=v.z; wz=v.w; break;
    case 3:
        xw=v.x; yw=v.y; zw=v.z; ww=v.w; break;
    default:
        std::cerr<<"Error: Try to set mat4.col("<<offset<<")"<<std::endl;
        assert(false);
    }
    return *this;
}

const float& mat4::operator[](std::size_t offset) const
{
    switch(offset) {

    case 0: return xx;
    case 1: return xy;
    case 2: return xz;
    case 3: return xw;

    case 4: return yx;
    case 5: return yy;
    case 6: return yz;
    case 7: return yw;

    case 8: return zx;
    case 9: return zy;
    case 10: return zz;
    case 11: return zw;

    case 12: return wx;
    case 13: return wy;
    case 14: return wz;
    case 15: return ww;

    default:
        std::cerr<<"Error: Try to access mat4["<<offset<<"]"<<std::endl;
        assert(false);
    }
	abort();
}
float& mat4::operator[](std::size_t offset)
{
    switch(offset) {

    case 0: return xx;
    case 1: return xy;
    case 2: return xz;
    case 3: return xw;

    case 4: return yx;
    case 5: return yy;
    case 6: return yz;
    case 7: return yw;

    case 8: return zx;
    case 9: return zy;
    case 10: return zz;
    case 11: return zw;

    case 12: return wx;
    case 13: return wy;
    case 14: return wz;
    case 15: return ww;

    default:
        std::cerr<<"Error: Try to access mat4["<<offset<<"]"<<std::endl;
        assert(false);
    }
	abort();
}

const float& mat4::operator()(std::size_t index1, std::size_t index2) const
{
    switch(index1) {
    case 0:
        switch(index2) {
        case 0: return xx;
        case 1: return xy;
        case 2: return xz;
        case 3: return xw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 1:
        switch(index2) {
        case 0: return yx;
        case 1: return yy;
        case 2: return yz;
        case 3: return yw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 2:
        switch(index2) {
        case 0: return zx;
        case 1: return zy;
        case 2: return zz;
        case 3: return zw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 3:
        switch(index2) {
        case 0: return wx;
        case 1: return wy;
        case 2: return wz;
        case 3: return ww;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    default:
        std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
        assert(false);
    }
	abort();
}
float& mat4::operator()(std::size_t index1, std::size_t index2)
{
    switch(index1) {
    case 0:
        switch(index2) {
        case 0: return xx;
        case 1: return xy;
        case 2: return xz;
        case 3: return xw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 1:
        switch(index2) {
        case 0: return yx;
        case 1: return yy;
        case 2: return yz;
        case 3: return yw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 2:
        switch(index2) {
        case 0: return zx;
        case 1: return zy;
        case 2: return zz;
        case 3: return zw;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    case 3:
        switch(index2) {
        case 0: return wx;
        case 1: return wy;
        case 2: return wz;
        case 3: return ww;
        default:
            std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
            assert(false);
        }
    default:
        std::cerr<<"Error: Try to access mat4("<<index1<<","<<index2<<")"<<std::endl;
        assert(false);
    }
	abort();
}


vcl::mat3 mat4::mat3() const
{
    return submat<3,3>(*this,0,0);
}
mat4& mat4::set_mat3(const vcl::mat3& m)
{
    set_submat(*this,0,0,m);
    return *this;
}
vcl::vec3 mat4::vec3() const
{
    return mat2vec(submat<3,1>(*this,0,3));
}
mat4& mat4::set_vec3(const vcl::vec3& tr)
{
    set_submat(*this,0,3,vec2mat(tr));
    return *this;

}

mat4 mat4::from_mat3_vec3(const vcl::mat3& linear_block, const vcl::vec3& translation_block)
{
    mat4 M = mat4::identity();
    M.set_mat3(linear_block);
    M.set_vec3(translation_block);
    return M;
}



}
