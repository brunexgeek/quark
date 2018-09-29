#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <getopt.h>
#include <stdint.h>
#include <cstring>

#include <engine/Vector3.hh>


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


struct WavefrontObject
{
	std::vector<Vector3f> vertices;
	std::vector<int> materials;
	std::vector<Vector3f> normals;
	std::vector< std::vector<int> > faces;
	std::vector<WavefrontMaterial*> materialLibrary;


    char *cleanup( char *line )
    {
        // ignore spaces from the begin
        char *ptr = line;
        while (*ptr == ' ' || *ptr == '\t') ++ptr;
        if (*ptr == 0) return nullptr;
        // remove line control characters from the end
        size_t len = strlen(ptr);
        if (len == 0) return nullptr;
        for (int i = len - 1; i >= 0 && ptr[i] < ' '; --i) ptr[i] = 0;

        std::cout << "Line: '" << ptr << "'" << std::endl;
        if (*ptr == 0) return nullptr;

        return ptr;
    }


	void splitText(
		const char *text,
		std::vector<std::string> &words )
	{

		const char *p = text;
		const char *e = text + strlen(text);

		while (p < e)
		{
			size_t pos = (size_t) strpbrk(p, " \t/");
			if (pos > 0)
				pos -= (size_t) p;
			else
				pos = strlen(p);

			if (pos == 0)
				++p;
			else
			{
				// whether we found a line comment, just ignores the rest of the line
				if (p[0] == '#') return;
				// stores the current word
				words.push_back( std::string(p, pos) );
				//std::cout << string(p, pos) << std::endl;
				p += pos + 1;
			}
		}
	}


	void loadMaterials(
		std::istream &in )
	{
		if (in.good() == false)
			throw std::string("Unable to read data from Wavefront input");

		char line[128];
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
			splitText(ptr, content);

			if (content.size() == 0) continue;

			if (content[0] == "newmtl" && content.size() == 2)
			{
				material = new WavefrontMaterial(content[1]);
				materialLibrary.push_back(material);
				std::cout << "Found material '" << content[1] << "'\n";
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

	void loadObject(
		std::istream &in )
	{
		if (in.good() == false)
			throw std::string("Unable to read data from Wavefront input");

		char line[128];
		std::vector<std::string> content;
		int currentMaterial = 0;

		while (in.good())
		{
			line[0] = 0;
			in.getline(line, sizeof(line) - 1);
            char *ptr = cleanup(line);
            if (ptr == nullptr) continue;

			// parses the line content
			content.clear();
			splitText(ptr, content);

			if (content.size() == 0) continue;

			// handles vertices
			if (content[0] == "v" && content.size() == 4)
			{
				Vector3f vertex;
				vertex.x = atof(content[1].c_str());
				vertex.y = atof(content[2].c_str());
				vertex.z = atof(content[3].c_str());
				vertices.push_back(vertex);
				materials.push_back(currentMaterial);
			}
			else
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
			if (content[0] == "f")
			{
				faces.resize( faces.size() + 1 );
				int index = faces.size() - 1;
				faces[index].resize(content.size() - 1);

				for (int i = 1, t = content.size(); i < t; ++i)
					faces[index][i - 1] = atoi(content[i].c_str());
			}
		}
	}

	WavefrontObject(
		std::istream &object,
		std::istream &materials )
	{
		loadMaterials(materials);
		loadObject(object);
	}
};


struct BinaryObject
{

};


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
	std::cout << "   Vertices: " << source.vertices.size() << std::endl;
	std::cout << "    Normals: " << source.normals.size() << std::endl;
	std::cout << "  Triangles: " << source.faces.size() << std::endl;
	std::cout << "  Materials: " << source.materialLibrary.size() << std::endl;

	/*std::ofstream outputFile(outputFileName.c_str());
	uint32_t count = (uint32_t) source.vertices.size();
	outputFile.write( (char*) &count, sizeof(uint32_t));
	outputFile.close();*/
}
