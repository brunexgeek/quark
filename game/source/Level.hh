#ifndef ZERK_LEVEL_HH
#define ZERK_LEVEL_HH


#include <stdint.h>
#include <string>


class Level
{
    public:
        ~Level();

        static Level *load(
            const std::string &fileName );

        const uint32_t *getData() const;

    private:
        uint32_t *data_;
        int width_, height_;

        Level(
            uint32_t *data,
            int width,
            int height );

};



#endif // ZERK_LEVEL_HH