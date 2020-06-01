#include "texture_gpu.hpp"

namespace vcl
{

//GLuint create_texture_gpu(const image& im)
//{
//    return create_texture_gpu(im.data, im.width, im.height);
//}

GLuint create_texture_gpu(std::vector<unsigned char> const& data, GLsizei width, GLsizei height, GLint wrap_s, GLint wrap_t)
{
    GLuint id = 0;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);

    // Send texture on GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set default texture behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D,0);

    return id;
}

GLuint create_texture_gpu(image_raw const& im, GLint wrap_s, GLint wrap_t)
{
    return create_texture_gpu(im.data, GLsizei(im.width), GLsizei(im.height), wrap_s, wrap_t);
}

//void update_texture_gpu(GLuint texture_id, image_rgb const& im)
//{
//    assert(glIsTexture(texture_id));

//    glBindTexture(GL_TEXTURE_2D, texture_id);
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, GLsizei(im.size()[0]), GLsizei(im.size()[1]), GL_RGB, GL_UNSIGNED_BYTE, &im.data[0][0]);
//    glGenerateMipmap(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,0);

////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
////    draw(quad, scene.camera);
////    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
//}


GLuint create_texture_gpu(buffer2D<vec3> const& im, GLint wrap_s, GLint wrap_t)
{
    GLuint id = 0;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);

    // Send texture on GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GLsizei(im.dimension[0]), GLsizei(im.dimension[1]), 0, GL_RGB, GL_FLOAT, &im.data[0][0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set default texture behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glBindTexture(GL_TEXTURE_2D,0);

    return id;
}
void update_texture_gpu(GLuint texture_id, buffer2D<vec3> const& im)
{
    assert_vcl(glIsTexture(texture_id), "Incorrect texture id");

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, GLsizei(im.dimension[0]), GLsizei(im.dimension[1]), GL_RGB, GL_FLOAT, &im.data[0][0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
}



}
