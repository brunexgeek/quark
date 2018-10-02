#include <engine/Mesh.hh>
#include <engine/Exception.hh>
#include <GL/glew.h>
#include <engine/Vector3.hh>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cstring>


using std::string;
using std::vector;
using std::istream;


Mesh::Mesh(
	const std::vector<Vector3f> &vertex,
	const std::vector<Vector3f> &normal,
	const std::vector<Vector2f> &uv,
	const std::vector<Vector3u> &faces,
	bool isDynamic )
{
	populate(vertex, normal, uv, faces, isDynamic);
}


Mesh::Mesh(
	std::istream &in,
	bool isDynamic )
{
	std::vector<Vector3f> vertex;
	std::vector<Vector2f> uv;
	std::vector<Vector3f> normal;
	std::vector<Vector3u> faceIndex;

	glGenBuffers(1, &vertexHandle_);
	glGenBuffers(1, &normalHandle_);
	glGenBuffers(1, &uvHandle_);
	glGenBuffers(1, &faceHandle_);

	//loadBin(in, Vector3f, color, normal, count);
	loadBinary(in, vertex, normal, uv, faceIndex);
	populate(vertex, normal, uv, faceIndex, isDynamic);
}


Mesh::Mesh(
	const std::string &fileName,
	bool isDynamic )
{
	std::ifstream input(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
	if (input.good())
	{
		initialize(input, isDynamic);
		input.close();
	}
}


Mesh::Mesh( Mesh &&obj )
{
	vertexHandle_ = obj.vertexHandle_;
	uvHandle_     = obj.uvHandle_;
	normalHandle_ = obj.normalHandle_;
	faceHandle_   = obj.faceHandle_;
	vertexCount_  = obj.vertexCount_;
	normalCount_  = obj.normalCount_;
	faceCount_    = obj.faceCount_;
	isDynamic     = obj.isDynamic;
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexHandle_);
	glDeleteBuffers(1, &normalHandle_);
	glDeleteBuffers(1, &uvHandle_);
	glDeleteBuffers(1, &faceHandle_);
}


void Mesh::initialize(
	std::istream &in,
	bool isDynamic )
{
	std::vector<Vector3f> vertex;
	std::vector<Vector2f> uv;
	std::vector<Vector3f> normal;
	std::vector<Vector3u> faceIndex;

	glGenBuffers(1, &vertexHandle_);
	glGenBuffers(1, &normalHandle_);
	glGenBuffers(1, &uvHandle_);
	glGenBuffers(1, &faceHandle_);

	//loadBin(in, Vector3f, color, normal, count);
	loadBinary(in, vertex, normal, uv, faceIndex);
	populate(vertex, normal, uv, faceIndex, isDynamic);
}


void Mesh::populate(
	const std::vector<Vector3f> &vertices,
	const std::vector<Vector3f> &normals,
	const std::vector<Vector2f> &uvs,
	const std::vector<Vector3u> &faces,
	bool isDynamic )
{
	this->vertexCount_ = (uint32_t) vertices.size();
	this->normalCount_ = (uint32_t) normals.size();
	this->faceCount_   = (uint32_t) faces.size();
	this->isDynamic    = isDynamic;

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexHandle_);
	glBufferData(GL_ARRAY_BUFFER, Vector3f::SIZE * vertices.size(), vertices.data(),
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	// normal buffer
	glGenBuffers(1, &normalHandle_);
	glBindBuffer(GL_ARRAY_BUFFER, normalHandle_);
	glBufferData(GL_ARRAY_BUFFER, Vector3f::SIZE * normals.size(), normals.data(),
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	// texture coordinate buffer
	glGenBuffers(1, &uvHandle_);
	glBindBuffer(GL_ARRAY_BUFFER, uvHandle_);
	glBufferData(GL_ARRAY_BUFFER, Vector2f::SIZE * uvs.size(), uvs.data(),
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	// vertex/normal indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceHandle_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Vector3u::SIZE * faces.size(), faces.data(),
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}


Vector3f *Mesh::computeNormal(
	const Vector3f *vertices,
	uint32_t count ) const
{
	(void) vertices;
	(void) count;
	// TODO: implement this!
	return NULL;
}


// FIXME: validate header/section signature
void Mesh::loadBinary(
	std::istream &in,
	std::vector<Vector3f> &vertices,
	std::vector<Vector3f> &normals,
	std::vector<Vector2f> &uvs,
	std::vector<Vector3u> &faceIndex )
{
	if (in.good() == false)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");

	in.seekg(0x00000008, in.beg);
	uint32_t total;

	// reads the vertices
	in.read( (char*) &total, sizeof(uint32_t) );
	if (in.good() == false || total > 0x7FFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");
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
