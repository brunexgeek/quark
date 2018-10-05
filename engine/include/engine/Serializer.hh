#ifndef ENGINE_SERIALIZER_HH
#define ENGINE_SERIALIZER_HH


#include <stdint.h>
#include <iostream>


class Serializer
{
    public:
        static void readBuffer(std::istream &in, uint8_t *buffer, size_t size );
        static int16_t readSint16(std::istream &in );
        static int32_t readSint32(std::istream &in );
        static int64_t readSint64(std::istream &in );
        static uint16_t readUint16(std::istream &in );
        static uint32_t readUint32(std::istream &in );
        static uint64_t readUint64(std::istream &in );

        static void writeBuffer(std::ostream &out, const uint8_t *buffer, size_t size );
        static void writeSint16(std::ostream &out, int16_t value );
        static void writeSint32(std::ostream &out, int32_t value );
        static void writeSint64(std::ostream &out, int64_t value );
        static void writeUint16(std::ostream &out, uint16_t value );
        static void writeUint32(std::ostream &out, uint32_t value );
        static void writeUint64(std::ostream &out, uint64_t value );

    private:
        Serializer();
};


#endif // ENGINE_SERIALIZER_HH