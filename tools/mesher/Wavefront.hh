#ifndef MESHER_WAVEFRONT_HH
#define MESHER_WAVEFRONT_HH


#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <engine/Vector.hh>


using quark::Vector3f;
using quark::Vector3u;
using quark::Vector2f;


struct WavefrontMaterial
{
	Vector3f colorDiffuse;
	Vector3f colorSpecular;
	Vector3f colorAmbient;
	std::string name;
	std::string texture;

	WavefrontMaterial(
		std::string name ) : name(name)
	{
		// nothing to do
	}

	WavefrontMaterial(
		const std::string &name,
		const std::string &texture ) : name(name), texture(texture)
	{
		// nothing to do
	}

};


struct WavefrontModel;


typedef void (WavefrontParser)(
	const std::vector<std::string> &content,
	WavefrontModel &model,
	void *context );


struct WavefrontFace
{
	uint32_t vertices[3];
	uint32_t normals[3];
	uint32_t uvs[3];
};


struct WavefrontObject
{
	std::string name;
	std::string material;
	std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<Vector2f> uvs;
	std::list<WavefrontFace> faces;

	WavefrontObject( const std::string &name );
	WavefrontObject( WavefrontObject&& object );
};


struct WavefrontModel
{
	std::unordered_map<std::string, WavefrontMaterial*> materialLibrary;
	std::list<WavefrontObject*> objects;
	uint32_t vertexCount = 0;
    uint32_t uvCount = 0;
    uint32_t normalCount = 0;
	uint32_t faceCount = 0;

    WavefrontModel(
		const WavefrontModel &obj ) = delete;

    WavefrontModel(
		WavefrontModel&& );

    static char *cleanup( char *line );

	static void split(
		char *text,
		std::vector<std::string> &words,
		const char separator = ' ' );

	void parseFile(
		const std::string &fileName,
		WavefrontParser *parser,
        void *context );

	void parseFile(
		std::istream &in,
		WavefrontParser *parser,
        void *context );


	static Vector3u getTriple(
		const std::string &triple );

	static WavefrontModel load(
		const std::string &fileName );

    private:
        WavefrontModel() {}

};


#endif // MESHER_WAVEFRONT_HH