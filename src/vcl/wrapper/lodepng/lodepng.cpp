#include "lodepng.hpp"

#include <iostream>

namespace vcl
{


image_raw image_load_png(const std::string& filename, image_color_type color_type)
{
    assert_file_exist(filename);

    LodePNGColorType lodepng_color_type;
    if(color_type==image_color_type::rgb)
        lodepng_color_type = LCT_RGB;
    else if(color_type==image_color_type::rgba)
        lodepng_color_type = LCT_RGBA;
    else
    {
        std::cerr<<"Unkown color type for file"<<filename<<std::endl;
        exit(1);
    }

    image_raw im;
    im.color_type = color_type;

    unsigned error = lodepng::decode(im.data, im.width, im.height, filename, lodepng_color_type);
    if ( error )
    {
        std::cerr<<"Error Loading png file "<<filename<<std::endl;
        std::cerr<<"Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        exit(1);
    }

    return im;
}

void image_save_png(const std::string& filename, const image_raw& im)
{
    LodePNGColorType lodepng_color_type;
    if(im.color_type==image_color_type::rgb)
        lodepng_color_type = LCT_RGB;
    else if(im.color_type==image_color_type::rgba)
        lodepng_color_type = LCT_RGBA;
    else
    {
        std::cerr<<"Unkown color type for file"<<filename<<std::endl;
        exit(1);
    }

    //std::vector<unsigned char> output;
    unsigned error = lodepng::encode(filename, im.data, im.width, im.height, lodepng_color_type);
    if ( error )
    {
        std::cerr<<"Error Loading png file "<<filename<<std::endl;
        std::cerr<<"Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        exit(1);
    }


}


}
