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


#define MESHER_SIGNATURE         (uint32_t) 0x4853454D
#define MESHER_OBJECT_SIGNATURE  (uint32_t) 0x544A424F
#define MESHER_VERTEX_SIGNATURE  (uint32_t) 0x54524556
#define MESHER_NORMAL_SIGNATURE  (uint32_t) 0x4D524F4E
#define MESHER_UV_SIGNATURE      (uint32_t) 0x20205655
#define MESHER_FACE_SIGNATURE    (uint32_t) 0x45434146


namespace quark {


MesherModel::MesherModel()
{
}


MesherModel::MesherModel(
	std::istream &in )
{
	load(in);
}


MesherModel::MesherModel(
	const std::string &fileName )
{
	std::ifstream input(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
	if (input.good())
	{
		load(input);
		input.close();
	}
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
	WRITE_U16(out, 0x0001);           // format
	WRITE_U16(out, objects.size());   // number of objects

	char name[MESHER_MAX_OBJ_NAME + 1];

	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		// object header
		Serializer::writeUint32(out, MESHER_OBJECT_SIGNATURE);
		Serializer::writeUint32(out, (uint32_t) it->faces->size());
		strncpy(name, it->name.c_str(), MESHER_MAX_OBJ_NAME);
		Serializer::writeBuffer(out, (const uint8_t*) name, MESHER_MAX_OBJ_NAME);

		// vertices
		Serializer::writeUint32(out, MESHER_VERTEX_SIGNATURE);
		Serializer::writeUint32(out, (uint32_t) it->vertices->size());
		Serializer::writeBuffer(out, (const uint8_t*) it->vertices->data(), it->vertices->size() * sizeof(Vector3f));

		// normals
		Serializer::writeUint32(out, MESHER_NORMAL_SIGNATURE);
		Serializer::writeUint32(out, (uint32_t) it->normals->size());
		Serializer::writeBuffer(out, (const uint8_t*) it->normals->data(), it->normals->size() * sizeof(Vector3f));

		// UVs
		Serializer::writeUint32(out, MESHER_UV_SIGNATURE);
		Serializer::writeUint32(out, (uint32_t) it->uvs->size());
		Serializer::writeBuffer(out, (const uint8_t*) it->uvs->data(), it->uvs->size() * sizeof(Vector2f));

		// faces
		Serializer::writeUint32(out, MESHER_FACE_SIGNATURE);
		Serializer::writeUint32(out, (uint32_t) it->faces->size());
		Serializer::writeBuffer(out, (const uint8_t*) it->faces->data(), it->faces->size() * sizeof(Vector3u));
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


void MesherModel::load(
	std::istream &in )
{
	if (!in.good())
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");

	// header
	if (Serializer::readUint32(in) != MESHER_SIGNATURE)
		throw EXCEPTION(ERR_IO_READ, 0, "Invalid file signature");
	if (Serializer::readUint16(in) != MESHER_FORMAT_VNU_24)
		throw EXCEPTION(ERR_IO_READ, 0, "Invalid mesh format");
	uint32_t objectCount = Serializer::readUint16(in);

	char name[MESHER_MAX_OBJ_NAME + 1];

	for (uint32_t i = 0; i < objectCount; ++i)
	{
		// object header
		if (Serializer::readUint32(in) != MESHER_OBJECT_SIGNATURE)
			throw EXCEPTION(ERR_IO_READ, 0, "Invalid object signature");
		uint32_t total = Serializer::readUint32(in);
		Serializer::readBuffer(in, (uint8_t*) name, MESHER_MAX_OBJ_NAME);

		MesherObject object;

		// vertices
		if (Serializer::readUint32(in) != MESHER_VERTEX_SIGNATURE)
			throw EXCEPTION(ERR_IO_READ, 0, "Invalid vertex signature");
		total = Serializer::readUint32(in);
		object.vertices->resize(total);
		Serializer::readBuffer(in, (uint8_t*) object.vertices->data(), total * sizeof(Vector3f));

		// normals
		if (Serializer::readUint32(in) != MESHER_NORMAL_SIGNATURE)
			throw EXCEPTION(ERR_IO_READ, 0, "Invalid normal signature");
		total = Serializer::readUint32(in);
		object.normals->resize(total);
		Serializer::readBuffer(in, (uint8_t*) object.normals->data(), total * sizeof(Vector3f));

		// UVs
		if (Serializer::readUint32(in) != MESHER_UV_SIGNATURE)
			throw EXCEPTION(ERR_IO_READ, 0, "Invalid UV signature");
		total = Serializer::readUint32(in);
		object.uvs->resize(total);
		Serializer::readBuffer(in, (uint8_t*) object.uvs->data(), total * sizeof(Vector2f));

		// faces
		if (Serializer::readUint32(in) != MESHER_FACE_SIGNATURE)
			throw EXCEPTION(ERR_IO_READ, 0, "Invalid face signature");
		total = Serializer::readUint32(in);
		object.faces->resize(total);
		Serializer::readBuffer(in, (uint8_t*) object.faces->data(), total * sizeof(Vector3u));

		objects.push_back(object);
	}
}

}