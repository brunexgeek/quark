#include <engine/Mesh.hh>
#include <engine/Exception.hh>
#include <GL/glew.h>
#include <engine/Vector3.hh>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>


using std::string;
using std::vector;
using std::istream;

/*
Mesh::Mesh(
	std::vector<Vector3f> &vertex,
	std::vector<Vector3f> &normal,
	std::vector<Vector3u> &faces,
	bool isDynamic ) : faceCount_(0), , isDynamic(isDynamic)
{
	//initialize(vertex, normal, faces, isDynamic);
}*/



Mesh::Mesh(
	std::istream &in,
	bool isDynamic )
{
	std::vector<Vector3f> vertex;
	std::vector<Vector3f> normal;
	std::vector<Vector3u> faceIndex;

	glGenBuffers(1, &vertexHandle_);
	glGenBuffers(1, &normalHandle_);
	glGenBuffers(1, &faceHandle_);

	//loadBin(in, Vector3f, color, normal, count);
	loadBinary(in, vertex, normal, faceIndex);
	populate(vertex, normal, faceIndex, isDynamic);
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexHandle_);
	glDeleteBuffers(1, &normalHandle_);
	glDeleteBuffers(1, &faceHandle_);
	//glDeleteBuffers(1, &colorsId_);
}


void Mesh::populate(
	std::vector<Vector3f> &vertices,
	std::vector<Vector3f> &normals,
	std::vector<Vector3u> &faces,
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

	// vertex/normal indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceHandle_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Vector3u::SIZE * faces.size(), faces.data(),
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	/*glGenBuffers(1, &colorId);
	glBindBuffer(GL_ARRAY_BUFFER, colorId);
	glBufferData(GL_ARRAY_BUFFER, Vector3f::SIZE * count, color,
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);*/

	/*glGenBuffers(1, &normalId);
	glBindBuffer(GL_ARRAY_BUFFER, normalId);
	glBufferData(GL_ARRAY_BUFFER, Vector3f::SIZE * normal.size(), normal.data(),
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);*/
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

/*
void Mesh::setVertex(
	const Vector3f *Vector3f,
	uint32_t begin,
	uint32_t size )
{
	setData(vertexId, (uint8_t*) Vector3f, begin, size, sizeof(Vector3f));
}


void Mesh::setNormal(
	const Vector3f *normal,
	uint32_t begin,
	uint32_t size )
{
	setData(normalId, (uint8_t*) normal, begin, size, sizeof(Vector3f));
}


void Mesh::setColor(
	const Vector3f *color,
	uint32_t begin,
	uint32_t size )
{
	setData(colorId, (uint8_t*) color, begin, size, sizeof(Vector3f));
}


void Mesh::setData(
	uint32_t id,
	const uint8_t *data,
	uint32_t start,
	uint32_t count,
	uint32_t size )
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
//std::cout << "Copy " << this->count << "x" << size << std::endl;
	if (start == 0 && count == 0)
		glBufferData(GL_ARRAY_BUFFER, this->count * size, data,
			(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	else
	{
		if (count == 0) count = this->count - start;

		glBufferSubData(GL_ARRAY_BUFFER, size * start, size * count, data);
	}
}
*/


void Mesh::loadBinary(
	std::istream &in,
	std::vector<Vector3f> &vertices,
	std::vector<Vector3f> &normals,
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
