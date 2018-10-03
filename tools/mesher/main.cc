#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <getopt.h>
#include <stdint.h>
#include <cstring>
#include <list>

#include <engine/Quaternion.hh>
#include <engine/Vector.hh>
#include <engine/Mesher.hh>
#include "Wavefront.hh"


std::string inputFileName;
std::string outputFileName;


#define WRITE_U32(out, value) do { uint32_t u32_temp_value = (uint32_t)value; (out).write( (char*) &u32_temp_value, sizeof(uint32_t)); } while(false)

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


static void main_printWavefront(
	std::ostream &out,
	const WavefrontModel &model )
{
	out.setf( std::ios::fixed, std:: ios::floatfield );

	for (auto obj = model.objects.begin(); obj != model.objects.end(); ++obj)
	{
		WavefrontObject &object = **obj;
		out << "o " << object.name << std::endl;

		// vertex
		out << std::setprecision(6);
		for (auto it = object.vertices.begin(); it != object.vertices.end(); ++it )
			out << "v " << it->x << ' ' << it->y << ' ' << it->z << std::endl;
		// UV
		out << std::setprecision(6);
		for (auto it = object.uvs.begin(); it != object.uvs.end(); ++it )
			out << "vt " << it->x << ' ' << it->y << std::endl;
		// normal
		out << std::setprecision(4);
		for (auto it = object.normals.begin(); it != object.normals.end(); ++it )
			out << "vn " << it->x << ' ' << it->y << ' ' << it->z << std::endl;
		// material
		out << "usemtl " << object.material << std::endl;
		// face
		for (auto it = object.faces.begin(); it != object.faces.end(); ++it )
		{
			out << "f";
			for (size_t i = 0; i < 3; ++i)
				out << ' ' << it->vertices[i]
					<< '/' << it->uvs[i]
					<< '/' << it->normals[i];
			out << std::endl;
		}
	}
}

#if 0
static void main_writeBinary(
	std::ostream &out,
	const WavefrontObject &object )
{
	uint32_t total = object.faces.size();

	// file header
	WRITE_U32(out, 0x4853454D);

	// vertex section
	WRITE_U32(out, 0x54524556);
	WRITE_U32(out, object.faces.size());
	for (auto it = object.faces.begin(); it != object.faces.end(); ++it )
		WRITE_V3F(out, it->vertex);

	// normal section
	WRITE_U32(out, 0x4D524F4E);
	WRITE_U32(out, object.faces.size());
	for (auto it = object.faces.begin(); it != object.faces.end(); ++it )
		WRITE_V3F(out, it->normal);

	// texture coordinates section
	WRITE_U32(out, 0x20205655);
	WRITE_U32(out, object.faces.size());
	for (auto it = object.faces.begin(); it != object.faces.end(); ++it )
		WRITE_V2F(out, it->uv);

	// face section
	WRITE_U32(out, 0x45434146);
	WRITE_U32(out, object.faces.size() / 3);
	uint32_t index = 0;
	for (auto it = object.faces.begin(); it != object.faces.end(); )
		for (size_t i = 0; i < 3 && it != object.faces.end(); ++i, ++it)
			WRITE_U32(out, index++);

	/*WRITE_U32(out, 0x5844494E);
	WRITE_U32(out, object.normalIndex.size());
	WRITE_U8P(out, object.normalIndex.data(), object.normalIndex.size() * sizeof(Vector3u));*/
}
#endif


static void main_usage()
{
    std::cerr << "Usage: mesher -i <object file> -m <materials file> -o <output file>\n";
    exit(EXIT_FAILURE);
}


static void main_parseOptions( int argc, char **argv )
{
    int opt;

    while ((opt = getopt(argc, argv, "i:o:")) != -1)
    {
        switch (opt)
        {
			case 'i':
				inputFileName = optarg;
				break;
			case 'o':
				outputFileName = optarg;
				break;
            default: /* '?' */
                main_usage();
        }
    }

    if (inputFileName.empty() || outputFileName.empty())
		main_usage();
}


static void main_createEntry(
	const WavefrontObject &source,
	MesherObject &dest,
    uint32_t vertexIndex,
    uint32_t uvIndex,
    uint32_t normalIndex )
{
	MesherFace face;
    face.vertex = source.vertices[vertexIndex];
    face.uv     = source.uvs[uvIndex];
    face.normal = source.normals[normalIndex];
	dest.faces.push_back(face);
}


int main( int argc, char **argv )
{
	main_parseOptions(argc, argv);

	std::map<std::string, WavefrontMaterial> materials;

	WavefrontModel source = WavefrontModel::load(inputFileName.c_str());

	std::cout << std::endl << "### Input file ###" << std::endl;
	std::cout << "   Vertices: " << source.vertexCount << std::endl;
	std::cout << "    Normals: " << source.normalCount << std::endl;
	std::cout << "        UVs: " << source.uvCount << std::endl;
	std::cout << "      Faces: " << source.faceCount / 3 << std::endl;
	std::cout << "  Materials: " << source.materialLibrary.size() << std::endl;

	//main_printWavefront(std::cerr, source);

	MesherModel dest;
	for (auto it = source.objects.begin(); it != source.objects.end(); ++it)
	{
		MesherObject object;
		object.name = (*it)->name;
		for (auto face = (*it)->faces.begin(); face != (*it)->faces.end(); ++face)
		{
			main_createEntry(*(*it), object, face->vertices[0] - 1, face->uvs[0] - 1, face->normals[0] - 1);
			main_createEntry(*(*it), object, face->vertices[1] - 1, face->uvs[1] - 1, face->normals[1] - 1);
			main_createEntry(*(*it), object, face->vertices[2] - 1, face->uvs[2] - 1, face->normals[2] - 1);
		}
		dest.objects.push_back(object);
	}

	dest.save(outputFileName);
/*
	std::cout << std::endl << "### Output file ###" << std::endl;
	std::cout << "   Vertices: " << source.faces.size() << std::endl;
	std::cout << "    Normals: " << source.faces.size() << std::endl;
	std::cout << "        UVs: " << source.faces.size() << std::endl;
	std::cout << "      Faces: " << source.faces.size() / 3 << std::endl;
	std::cout << "  Materials: " << source.materialLibrary.size() << std::endl;

	std::ofstream outputFile(outputFileName.c_str(), std::ios_base::ate | std::ios_base::binary);
	main_writeBinary(outputFile, source);
	//uint32_t count = (uint32_t) source.vertices.size();
	//outputFile.write( (char*) &count, sizeof(uint32_t));
	outputFile.close();*/
}
