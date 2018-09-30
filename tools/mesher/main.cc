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


std::string inputFileName;
std::string outputFileName;
std::string materialsFileName;


struct WavefrontMaterial
{
	Vector3f colorDiffuse;
	Vector3f colorSpecular;
	Vector3f colorAmbient;
	std::string name;

	WavefrontMaterial(
		std::string name ) : name(name)
	{
		// nothing to do
	}

};


struct WavefrontVertex
{
	//uint32_t index;
	Vector3f vertex;
	Vector3f normal;
	//Vector3f color;
	//Vector2f uv;
};


struct WavefrontObject
{
	//std::vector<Vector3u> faceIndex; // every element make a triangle
	//std::vector<Vector3u> normalIndex; // every element make a triangle normal
	uint32_t vertexCount;
	uint32_t normalCount;
	std::vector<WavefrontMaterial*> materialLibrary;
	std::list<WavefrontVertex> faces;


    char *cleanup( char *line )
    {
        // ignore spaces from the begin
        char *ptr = line;
        while (*ptr == ' ' || *ptr == '\t') ++ptr;
        if (*ptr == 0) return nullptr;
        // remove line control characters from the end
        size_t len = strlen(ptr);
        if (len == 0) return nullptr;
        for (int i = len - 1; i >= 0 && ptr[i] <= ' '; --i) ptr[i] = 0;

//std::cout << "Line: '" << ptr << "'" << std::endl;
        if (*ptr == 0 || *ptr == '#') return nullptr;

        return ptr;
    }


	static void split(
		char *text,
		std::vector<std::string> &words,
		const char separator = ' ' )
	{

		char *ptr = text;
		char *end = text;

		while (*end != 0)
		{
			// remove leading whitespaces
			while (*ptr == ' ' || *ptr == '\t') end = ++ptr;
			// look for the next whitespace
			//while (*end != ' ' && *end != '\t' && *end != 0) ++end;
			while (*end != separator && *end != 0) ++end;

			if (*end == 0)
			{
				if (*ptr == 0) break;
				words.push_back(ptr);
//std::cout << "   '" << ptr << "'" << std::endl;
				break;
			}
			else
			{
				*end = 0;
//std::cout << "   '" << ptr << "'" << std::endl;
				words.push_back(ptr);
				ptr = end = end + 1;
			}
		}
	}


	void loadMaterials(
		std::istream &in )
	{
		if (in.good() == false)
			throw std::string("Unable to read data from Wavefront input");

		char line[1024];
		std::vector<std::string> content;

		WavefrontMaterial *material = NULL;

		while (in.good())
		{
			line[0] = 0;
			in.getline(line, sizeof(line) - 1);
            char *ptr = cleanup(line);
            if (ptr == nullptr) continue;

			// parses the line content
			content.clear();
			split(ptr, content);
			continue;

			if (content.size() == 0) continue;

			if (content[0] == "newmtl" && content.size() == 2)
			{
				material = new WavefrontMaterial(content[1]);
				materialLibrary.push_back(material);
//std::cout << "Found material '" << content[1] << "'\n";
			}
			else
			if (content[0] == "Kd" && content.size() == 4 && material != NULL)
			{
				material->colorDiffuse.x = atof(content[1].c_str());
				material->colorDiffuse.y = atof(content[2].c_str());
				material->colorDiffuse.z = atof(content[3].c_str());
			}
		}
	}

	static Vector3u getTriple( const std::string &triple )
	{
		std::vector<std::string> values;
		char temp[512];
		strncpy(temp, triple.c_str(), sizeof(temp)-1);
//std::cout << "  Face: '" << triple << "'" << std::endl;
		split(temp, values, '/');
		Vector3u current = { 0, 0, 0 };
		switch (values.size())
		{
			case 3: current.z = atoi(values[2].c_str());
			case 2: current.y = atoi(values[1].c_str());
			case 1: current.x = atoi(values[0].c_str());
			default: break;
		}
//std::cout << "    Vector: '" << current.x << ", " << current.y << ", " << current.z << "'" << std::endl;
		return current;
	}

	void loadObject(
		std::istream &in )
	{
		uint32_t indexCounter = 0;
		std::vector<int> materials;
		std::vector<Vector3f> vertices;
		std::vector<Vector3f> normals;

		if (in.good() == false)
			throw std::string("Unable to read data from Wavefront input");

		char line[128];
		std::vector<std::string> content;
		int currentMaterial = -1;

		while (in.good())
		{
			line[0] = 0;
			in.getline(line, sizeof(line) - 1);
            char *ptr = cleanup(line);
            if (ptr == nullptr) continue;

			// parses the line content
			content.clear();
			split(ptr, content);
			//splitText(ptr, content);

			if (content.size() == 0) continue;

			// handles vertices
			if (content[0] == "v" && (content.size() == 4 || content.size() == 5))
			{
				Vector3f vertex;
				vertex.x = atof(content[1].c_str());
				vertex.y = atof(content[2].c_str());
				vertex.z = atof(content[3].c_str());
				/*if (content.size() == 5)
					vertex.w = atof(content[4].c_str());
				else
					vertex.w = 1.0F;*/
				vertices.push_back(vertex);
				materials.push_back(currentMaterial);
			}
			else
			#if 0
			// handles textures coordinates
			if (content[0] == "usemtl" && content.size() == 2)
			{
				currentMaterial = 0;
				for (int i = 0, t = materials.size(); i < t; ++i)
				{
					if (materialLibrary[i]->name == content[1])
					{
						currentMaterial = i;
						break;
					}
				}
			}
			else
			#endif
			// handles normals
			if (content[0] == "vn" && content.size() == 4)
			{
				Vector3f normal;
				normal.x = atof(content[1].c_str());
				normal.y = atof(content[2].c_str());
				normal.z = atof(content[3].c_str());
				normals.push_back(normal);
			}
			else
			// handles faces
			if (content[0] == "f" && content.size() == 4)
			{
				Vector3u first  = getTriple(content[1]);
				Vector3u second = getTriple(content[2]);
				Vector3u third  = getTriple(content[3]);

				createEntry(indexCounter++, faces, vertices, first.x - 1,  normals, first.z - 1);
				createEntry(indexCounter++, faces, vertices, second.x - 1, normals, second.z - 1);
				createEntry(indexCounter++, faces, vertices, third.x - 1,  normals, third.z - 1);

				//faceIndex.push_back(Vector3u(first.x - 1, second.x - 1, third.x - 1));
				//normalIndex.push_back(Vector3u(first.z - 1, second.z - 1, third.z - 1));
			}
		}
		vertexCount = vertices.size();
		normalCount = normals.size();
	}

	void createEntry(
		uint32_t index,
		std::list<WavefrontVertex> &out,
		const std::vector<Vector3f> &vertices,
		const uint32_t vertexIndex,
		const std::vector<Vector3f> &normals,
		const uint32_t normalIndex )
	{
		WavefrontVertex current;
		//current.index = index;
		current.vertex = vertices[vertexIndex];
		current.normal = normals[normalIndex];
		out.push_back(current);
	}

/*
	std::list<WavefrontVertex> loadModel()
	{
		std::list<WavefrontVertex> entries;

		for (

	}
*/

	WavefrontObject(
		std::istream &object,
		std::istream &materials )
	{
		loadMaterials(materials);
		loadObject(object);
	}
};


#define WRITE_U32(out, value) do { uint32_t u32_temp_value = (uint32_t)value; (out).write( (char*) &u32_temp_value, sizeof(uint32_t)); } while(false)
#define WRITE_U8P(out, ptr, size) do { (out).write( (char*) ptr, size); } while(false)
#define WRITE_V3F(out, vec3f) \
	do { \
		(out).write( (char*) &(vec3f).x, sizeof(float)); \
		(out).write( (char*) &(vec3f).y, sizeof(float)); \
		(out).write( (char*) &(vec3f).z, sizeof(float)); \
	} while(false)

static void main_writeBinary(
	std::ostream &out,
	const WavefrontObject &object )
{
	WRITE_U32(out, 0x4853454D);

	uint32_t total = object.faces.size();

	WRITE_U32(out, 0x54524556);
	//WRITE_U32(out, object.vertices.size());
	//WRITE_U8P(out, object.vertices.data(), object.vertices.size() * sizeof(Vector3f));
	WRITE_U32(out, object.faces.size());
	for (auto it = object.faces.begin(); it != object.faces.end(); ++it )
	{
		WRITE_V3F(out, it->vertex);
		std::cerr << "v " << it->vertex << std::endl;
	}

	WRITE_U32(out, 0x4D524F4E);
	//WRITE_U32(out, object.normals.size());
	//WRITE_U8P(out, object.normals.data(), object.normals.size() * sizeof(Vector3f));
	WRITE_U32(out, object.faces.size());
	for (auto it = object.faces.begin(); it != object.faces.end(); ++it )
	{
		WRITE_V3F(out, it->normal);
		std::cerr << "vn " << it->normal << std::endl;
	}

	WRITE_U32(out, 0x46414345);
	//WRITE_U32(out, object.faceIndex.size());
	//WRITE_U8P(out, object.faceIndex.data(), object.faceIndex.size() * sizeof(Vector3u));
	WRITE_U32(out, object.faces.size() / 3);
	uint32_t index = 0;
	for (auto it = object.faces.begin(); it != object.faces.end(); )
	{
		std::cerr << "f";
		for (size_t i = 0; i < 3 && it != object.faces.end(); ++i, ++it)
		{
			WRITE_U32(out, index++);
			std::cerr <<  ' ' << index << "//" << index;
		}
		std::cerr << std::endl;
	}

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

    while ((opt = getopt(argc, argv, "i:o:m:")) != -1)
    {
        switch (opt)
        {
			case 'i':
				inputFileName = optarg;
				break;
			case 'm':
				materialsFileName = optarg;
				break;
			case 'o':
				outputFileName = optarg;
				break;
            default: /* '?' */
                main_usage();
        }
    }

    if (inputFileName.empty() || outputFileName.empty() || materialsFileName.empty())
		main_usage();
}



int main( int argc, char **argv )
{
	main_parseOptions(argc, argv);

	std::map<std::string, WavefrontMaterial> materials;

	std::ifstream objectFile(inputFileName.c_str());
	std::ifstream materialsFile(materialsFileName.c_str());
	WavefrontObject source(objectFile, materialsFile);
	objectFile.close();
	materialsFile.close();

	std::cout << std::endl << "### Input file ###" << std::endl;
	std::cout << "   Vertices: " << source.vertexCount << std::endl;
	std::cout << "    Normals: " << source.normalCount << std::endl;
	std::cout << "      Faces: " << source.faces.size() / 3 << std::endl;
	std::cout << "  Materials: " << source.materialLibrary.size() << std::endl;

	std::cout << std::endl << "### Output file ###" << std::endl;
	std::cout << "   Vertices: " << source.faces.size() << std::endl;
	std::cout << "    Normals: " << source.faces.size() << std::endl;
	std::cout << "      Faces: " << source.faces.size() / 3 << std::endl;
	std::cout << "  Materials: " << source.materialLibrary.size() << std::endl;

	std::ofstream outputFile(outputFileName.c_str(), std::ios_base::ate | std::ios_base::binary);
	main_writeBinary(outputFile, source);
	//uint32_t count = (uint32_t) source.vertices.size();
	//outputFile.write( (char*) &count, sizeof(uint32_t));
	outputFile.close();
}
