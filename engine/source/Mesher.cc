#include <engine/Mesher.hh>
#include <engine/Exception.hh>
#include <engine/Serializer.hh>
#include <fstream>


#define WRITE_U32(out, value) do { uint32_t u32_temp_value = (uint32_t)value; (out).write( (char*) &u32_temp_value, sizeof(uint32_t)); } while(false)

#define WRITE_U16(out, value) do { uint16_t u16_temp_value = (uint16_t)value; (out).write( (char*) &u16_temp_value, sizeof(uint16_t)); } while(false)

#define WRITE_U8P(out, ptr, size) do { (out).write( (char*) ptr, size); } while(false)

#define WRITE_V3F(out, vec3f) \
	do { \
		(out).write( (char*) &(vec3f).x, sizeof(float)); \
		(out).write( (char*) &(vec3f).y, sizeof(float)); \
		(out).write( (char*) &(vec3f).z, sizeof(float)); \
	} while(false)

#define WRITE_V2F(out, vec2f) \
	do { \
		(out).write( (char*) &(vec2f).x, sizeof(float)); \
		(out).write( (char*) &(vec2f).y, sizeof(float)); \
	} while(false)


#define MESHER_SIGNATURE         0x4853454D
#define MESHER_OBJECT_SIGNATURE  0x544A424F
#define MESHER_VERTEX_SIGNATURE  0x54524556
#define MESHER_NORMAL_SIGNATURE  0x4D524F4E
#define MESHER_UV_SIGNATURE      0x20205655
#define MESHER_FACE_SIGNATURE    0x45434146


MesherModel::MesherModel()
{
}


MesherModel::MesherModel(
    std::istream &in )
{
}


MesherModel::MesherModel(
    const std::string &fileName )
{

}


MesherModel& MesherModel::operator+=(
    MesherObject &object )
{
    objects.push_back(object);
    return *this;
}


void MesherModel::save(
	std::ostream &out )
{
    // file header
    WRITE_U32(out, MESHER_SIGNATURE); // signature
    WRITE_U16(out, 0x0001);     // format
    WRITE_U16(out, 0x0000);     // reserved

    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        // object header
        WRITE_U32(out, MESHER_OBJECT_SIGNATURE);

        // vertex section
        WRITE_U32(out, MESHER_VERTEX_SIGNATURE);
        WRITE_U32(out, it->faces.size());
        for (auto item = it->faces.begin(); item != it->faces.end(); ++item )
            WRITE_V3F(out, item->vertex);

        // normal section
        WRITE_U32(out, MESHER_NORMAL_SIGNATURE);
        WRITE_U32(out, it->faces.size());
        for (auto item = it->faces.begin(); item != it->faces.end(); ++item )
            WRITE_V3F(out, item->normal);

        // texture coordinates section
        WRITE_U32(out, MESHER_UV_SIGNATURE);
        WRITE_U32(out, it->faces.size());
        for (auto item = it->faces.begin(); item != it->faces.end(); ++item )
            WRITE_V2F(out, item->uv);

        // face section
        WRITE_U32(out, MESHER_FACE_SIGNATURE);
        WRITE_U32(out, it->faces.size() / 3);
        uint32_t index = 0;
        for (auto item = it->faces.begin(); item != it->faces.end(); ++item)
            for (size_t i = 0; i < 3; ++i) WRITE_U32(out, index++);
    }
}


void MesherModel::save(
	const std::string &fileName )
{
    std::ofstream output(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
    if (output.good())
    {
        save(output);
        output.close();
    }
}

#if 0
Complete this!
void MesherModel::load(
    std::istream &in )
{
	if (in.good() == false)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");

	uint32_t uiValue = 0;
    if (Serializer::readUint32(in) != MESHER_SIGNATURE)
        throw EXCEPTION(ERR_IO_READ, 0, "Invalid file signature");

	uint32_t total = 0;

	// reads the vertices
    total = Serializer::readUint32(in);
	if (!in.good() || total > 0x00FFFFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Invalid amount of vertices");
	if (total > 0)
	{
		vertices.resize(total);
		in.read( (char*) vertices.data(), total * sizeof(Vector3f) );
	}

	// reads the normals
	in.seekg(0x00000004, in.cur);
	in.read( (char*) &total, sizeof(uint32_t) );
	if (in.good() == false || total > 0x7FFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");
	if (total > 0)
	{
		normals.resize(total);
		in.read( (char*) normals.data(), total * sizeof(Vector3f) );
	}

	// reads the texture coordinates
	in.seekg(0x00000004, in.cur);
	in.read( (char*) &total, sizeof(uint32_t) );
	if (in.good() == false || total > 0x7FFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");
	if (total > 0)
	{
		uvs.resize(total);
		in.read( (char*) uvs.data(), total * sizeof(Vector2f) );
	}

	// reads the face indexes
	in.seekg(0x00000004, in.cur);
	in.read( (char*) &total, sizeof(uint32_t) );
	if (in.good() == false || total > 0x7FFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");
	if (total > 0)
	{
		faceIndex.resize(total);
		in.read( (char*) faceIndex.data(), total * sizeof(Vector3f) );
	}
	// reads the normal indexes
	/*in.seekg(0x00000004, in.cur);
	in.read( (char*) &total, sizeof(uint32_t) );
	if (in.good() == false || total > 0x7FFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");
	if (total > 0)
	{
		normalIndex.resize(total);
		in.read( (char*) normalIndex.data(), total * sizeof(Vector3f) );
	}*/

	std::cout << "Loaded model:" << std::endl;
	std::cout << "      Faces: " << faceIndex.size() << std::endl;
	std::cout << "   Vertices: " << vertices.size() << std::endl;
}
}
#endif