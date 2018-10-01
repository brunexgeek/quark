#include <engine/Texture.hh>


void Texture::initialize(
    int width,
    int height,
    const uint8_t *data )
{
    glGenTextures(1, &handler);
    glBindTexture(GL_TEXTURE_2D, handler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}