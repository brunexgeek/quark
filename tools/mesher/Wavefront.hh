#ifndef MESHER_WAVEFRONT_HH
#define MESHER_WAVEFRONT_HH


#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <engine/Vector.hh>


struct WavefrontMaterial
{
	uint32_t id;
	Vector3f colorDiffuse;
	Vector3f colorSpecular;
	Vector3f colorAmbient;
	std::string name;
	std::string texture;

	WavefrontMaterial(
		uint32_t id,
		std::string name ) : id(id), name(name)
	{
		// nothing to do
	}

	WavefrontMaterial(
		uint32_t id,
		const std::string &name,
		const std::string &texture ) : id(id), name(name), texture(texture)
	{
		// nothing to do
	}

};


struct WavefrontObject;


typedef void (WavefrontParser)(
	const std::vector<std::string> &content,
	WavefrontObject &object,
	void *context );


struct WavefrontVertex
{
	//uint32_t index;
	Vector3f vertex;
	Vector3f normal;
	//Vector3f color;
	Vector2f uv;
};


struct WavefrontObject
{
	//std::vector<Vector3u> faceIndex; // every element make a triangle
	//std::vector<Vector3u> normalIndex; // every element make a triangle normal
	uint32_t vertexCount;
	uint32_t normalCount;
	std::unordered_map<std::string, WavefrontMaterial*> materialLibrary;
	std::list<WavefrontVertex> faces;

    WavefrontObject( const WavefrontObject &obj ) = delete;

    WavefrontObject( WavefrontObject&& );

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


	static Vector3u getTriple( const std::string &triple );

	static WavefrontObject load(
		const std::string &fileName );

	void createEntry(
		uint32_t index,
		std::list<WavefrontVertex> &out,
		const std::vector<Vector3f> &vertices,
		const uint32_t vertexIndex,
		const std::vector<Vector2f> &uvs,
		const uint32_t uvIndex,
		const std::vector<Vector3f> &normals,
		const uint32_t normalIndex );

    private:
        WavefrontObject() {}

};


#endif // MESHER_WAVEFRONT_HH