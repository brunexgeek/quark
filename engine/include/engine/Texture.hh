#ifndef ENGINE_TEXTURE_HH
#define ENGINE_TEXTURE_HH


#include <stdint.h>
#include <fstream>
#include <GL/glew.h>


class Texture
{
    public:
        Texture(
            int width,
            int height,
            const uint8_t *data )
        {
            initialize(width, height, data);
        }

        Texture(
            int width,
            int height,
            const std::string &fileName )
        {
            std::ifstream input(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
            if (input.good())
            {
                input.seekg(0, std::ios_base::end);
                auto size = input.tellg();
                input.seekg(0, std::ios_base::beg);
                uint8_t *data = new(std::nothrow) uint8_t[size];
                if (data != nullptr)
                {
                    input.read( (char*)data, size);
                    initialize(width, height, data);
                    delete data;
                }
                input.close();
            }
        }

        ~Texture()
        {
            glDeleteTextures(1, &handler);
        }

        uint32_t getHandler() const { return handler; }

    private:
        uint32_t handler;

        void initialize(
            int width,
            int height,
            const uint8_t *data );
};


#endif // ENGINE_TEXTURE_HH