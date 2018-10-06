#include <engine/opengl/Texture.hh>


namespace quark {
namespace opengl {


void Texture::initialize(
    int width,
    int height,
    const uint8_t *data,
    bool isGrayScale )
{
    glGenTextures(1, &handler);
    glBindTexture(GL_TEXTURE_2D, handler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGB8;
    if (isGrayScale) GL_R3_G3_B2;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}


}}