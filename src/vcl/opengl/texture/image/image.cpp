#include "image.hpp"

#include "vcl/math/math.hpp"

namespace vcl
{

image_raw::image_raw()
    :width(0), height(0), color_type(image_color_type::rgb), data()
{}
image_raw::image_raw(unsigned int width_arg, unsigned int height_arg, image_color_type color_type_arg, std::vector<unsigned char> const& data_arg)
    :width(width_arg), height(height_arg), color_type(color_type_arg), data(data_arg)
{}

buffer2D<vec3> image_raw::to_buffer_rgb() const
{
    buffer2D<vec3> b;
    b.resize(width, height);

    for(size_t i=0; i<width; ++i){
        for(size_t j=0; j<height; ++j){
            size_t const k = i+width*j;

            if(color_type==image_color_type::rgb)
                b(i,height-1-j) = {data[3*k]/255.0f, data[3*k+1]/255.0f, data[3*k+2]/255.0f};
            else
                b(i,height-1-j) = {data[4*k]/255.0f, data[4*k+1]/255.0f, data[4*k+2]/255.0f};
        }
    }

    return b;
}

}
