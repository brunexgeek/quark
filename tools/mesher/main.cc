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
	const WavefrontObject &object )
{
	for (auto it = object.faces.begin(); it != object.faces.end(); ++it )
		std::cerr << "v " << it->vertex << std::endl;

	for (auto it = object.faces.begin(); it != object.faces.end(); ++it )
		std::cerr << "vn " << it->normal << std::endl;

	uint32_t index = 0;
	for (auto it = object.faces.begin(); it != object.faces.end(); )
	{
		std::cerr << "f";
		for (size_t i = 0; i < 3 && it != object.faces.end(); ++i, ++it)
			std::cerr <<  ' ' << index << "//" << index;
		std::cerr << std::endl;
	}
}


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



int main( int argc, char **argv )
{
	main_parseOptions(argc, argv);

	std::map<std::string, WavefrontMaterial> materials;

	WavefrontObject source = WavefrontObject::load(inputFileName.c_str());

	std::cout << std::endl << "### Input file ###" << std::endl;
	std::cout << "   Vertices: " << source.vertexCount << std::endl;
	std::cout << "    Normals: " << source.normalCount << std::endl;
	std::cout << "        UVs: " << source.uvCount << std::endl;
	std::cout << "      Faces: " << source.faces.size() / 3 << std::endl;
	std::cout << "  Materials: " << source.materialLibrary.size() << std::endl;

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
	outputFile.close();
}
