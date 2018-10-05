#include <engine/Serializer.hh>


void Serializer::readBuffer(std::istream &in, uint8_t *buffer, size_t size )
{
    in.read((char*)buffer, size);
}

int16_t Serializer::readSint16(std::istream &in )
{
    int16_t value;
    in.read((char*)&value, sizeof(value));
    return value;
}

int32_t Serializer::readSint32(std::istream &in )
{
    int32_t value;
    in.read((char*)&value, sizeof(value));
    return value;
}

int64_t Serializer::readSint64(std::istream &in )
{
    int64_t value;
    in.read((char*)&value, sizeof(value));
    return value;
}

uint16_t Serializer::readUint16(std::istream &in )
{
    uint16_t value;
    in.read((char*)&value, sizeof(value));
    return value;
}

uint32_t Serializer::readUint32(std::istream &in )
{
    uint32_t value;
    in.read((char*)&value, sizeof(value));
    return value;
}

uint64_t Serializer::readUint64(std::istream &in )
{
    uint64_t value;
    in.read((char*)&value, sizeof(value));
    return value;
}

void Serializer::writeBuffer(std::ostream &out, const uint8_t *buffer, size_t size )
{
    out.write((const char*)buffer, size);
}

void Serializer::writeSint16(std::ostream &out, int16_t value )
{
    out.write((const char*)&value, sizeof(value));
}

void Serializer::writeSint32(std::ostream &out, int32_t value )
{
    out.write((const char*)&value, sizeof(value));
}

void Serializer::writeSint64(std::ostream &out, int64_t value )
{
    out.write((const char*)&value, sizeof(value));
}

void Serializer::writeUint16(std::ostream &out, uint16_t value )
{
    out.write((const char*)&value, sizeof(value));
}

void Serializer::writeUint32(std::ostream &out, uint32_t value )
{
    out.write((const char*)&value, sizeof(value));
}

void Serializer::writeUint64(std::ostream &out, uint64_t value )
{
    out.write((const char*)&value, sizeof(value));
}
