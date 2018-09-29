#include "Level.hh"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Level *Level::load(
    const std::string &fileName )
{
    int width, height, comp;
    uint8_t *data = stbi_load(fileName.c_str(), &width, &height, &comp, 0);
    if (data == nullptr) return nullptr;
    std::cout << "Loaded map with " << comp << std::endl;
    size_t size = width * height * comp;
    uint32_t *temp = nullptr;

    if (comp == 4)
    {
        temp = new(std::nothrow) uint32_t[size];
        if (temp != nullptr) memcpy(temp, data, size);
        stbi_image_free(data);
    }

    if (temp == nullptr) return nullptr;
    return new Level(temp, width, height);
}


Level::Level(
    uint32_t *data,
    int width,
    int height ) : data_(data), width_(width), height_(height)
{
}


Level::~Level()
{
    delete data_;
}


const uint32_t *Level::getData() const
{
    return data_;
}