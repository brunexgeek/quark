#ifndef QUARK_TEXTURE_HH
#define QUARK_TEXTURE_HH


#include <stdint.h>
#include <fstream>
#include <GL/glew.h>


namespace quark {


class Texture
{
    public:
        Texture(
            int width,
            int height,
            const uint8_t *data ) : width_(width), height_(height)
        {
            (void) data;
        }

        Texture(
            int width,
            int height,
            const std::string &fileName ) : width_(width), height_(height)
        {
            (void) fileName;
        }

        virtual ~Texture() = 0;

        int getWidth() const   { return width_; }
        int getHeight() const   { return height_; }

    protected:
        int width_, height_;
};


}


#endif // QUARK_TEXTURE_HH