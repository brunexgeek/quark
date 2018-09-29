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


Mesh::Mesh(
	std::vector<Vector3f> &vertex,
	std::vector<Vector3f> &normal,
	std::vector<Vector3u> &faces,
	bool isDynamic ) : count(count), isDynamic(isDynamic)
{
	initialize(vertex, normal, faces, isDynamic);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexId);
	glDeleteBuffers(1, &colorId);
	glDeleteBuffers(1, &normalId);
}


void Mesh::initialize(
	std::vector<Vector3f> &vertex,
	std::vector<Vector3f> &normal,
	std::vector<Vector3u> &faces,
	bool isDynamic )
{
	this->count = count;
	this->isDynamic = isDynamic;

	glGenBuffers(1, &vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, Vector3f::SIZE * vertex.size(), vertex.data(),
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	glGenBuffers(1, &faceId_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceId_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Vector3u::SIZE * faces.size(), faces.data(),
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	faceCount = (uint32_t) faces.size();

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
	const Vector3f *vetices,
	uint32_t count ) const
{
	// TODO: implement this!
	return NULL;
}

uint32_t Mesh::getColorId() const
{
	return colorId;
}


uint32_t Mesh::getVertexCount() const
{
	return count;
}


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


static void splitText(
	const char *text,
	vector<string> &words )
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
			words.push_back( string(p, pos) );
			//std::cout << string(p, pos) << std::endl;
			p += pos + 1;
		}
	}
}


Mesh::Mesh(
	std::istream &in,
	bool isDynamic )
{
	std::vector<Vector3f> vertex;
	std::vector<Vector3f> normal;
	std::vector<Vector3u> faceIndex;
	std::vector<Vector3u> normalIndex;

	//loadBin(in, Vector3f, color, normal, count);
	loadBinary(in, vertex, normal, faceIndex, normalIndex);
	initialize(vertex, normal, faceIndex, isDynamic);
}


static uint32_t readUnsigned32(
	std::istream &in )
{
	uint32_t out;
	in.read( (char*) &out, sizeof(uint32_t) );
	return out;
}


void Mesh::loadBinary(
	std::istream &in,
	std::vector<Vector3f> &vertices,
	std::vector<Vector3f> &normals,
	std::vector<Vector3u> &faceIndex,
	std::vector<Vector3u> &normalIndex )
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
	in.seekg(0x00000004, in.cur);
	in.read( (char*) &total, sizeof(uint32_t) );
	if (in.good() == false || total > 0x7FFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");
	if (total > 0)
	{
		normalIndex.resize(total);
		in.read( (char*) normalIndex.data(), total * sizeof(Vector3f) );
	}

	std::cout << "Loaded model:" << std::endl;
	std::cout << "      Faces: " << faceIndex.size() << std::endl;
	std::cout << "   Vertices: " << vertices.size() << std::endl;
}

#if 0
void Mesh::loadObject(
	istream &in,
	Vector3f *&vertices,
	Vector3f *&colors,
	Vector3f *&normals,
	uint32_t &count )
{
	if (in.good() == false)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from Wavefront input");

static bool first = false;

	std::string line;
	std::vector<uint32_t> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3f> temp_vertices;
	//std::vector<glm::vec2> temp_uvs;
	std::vector<Vector3f> temp_normals;
	vector<string> content;

	while (in.good())
	{
		getline(in, line);

		// parses the line content
		content.clear();
		splitText(line.c_str(), content);

		if (content.size() == 0) continue;

		// handles vertices
		if (content[0] == "v" && content.size() == 4)
		{
			Vector3f Vector3f;
			Vector3f.x = atof(content[1].c_str());
			Vector3f.y = atof(content[2].c_str());
			Vector3f.z = atof(content[3].c_str());
			temp_vertices.push_back(Vector3f);
		}
		else
		// handles textures coordinates
		/*if (content[0] == "vt" && content.size() == 3)
		{
			glm::vec2 uv;
			uv.x = atof(content[1].c_str());
			uv.y = atof(content[2].c_str());
			temp_uvs.push_back(uv);
		}
		else */
		// handles normals
		if (content[0] == "vn" && content.size() == 4)
		{
			Vector3f normal;
			normal.x = atof(content[1].c_str());
			normal.y = atof(content[2].c_str());
			normal.z = atof(content[3].c_str());
			temp_normals.push_back(normal);
		}
		else
		// handles faces
		if (content[0] == "f")
		{
			if (content.size() == 10)
			{
				vertexIndices.push_back( atoi(content[1].c_str()) );
				vertexIndices.push_back( atoi(content[4].c_str()) );
				vertexIndices.push_back( atoi(content[7].c_str()) );

				uvIndices    .push_back( atoi(content[2].c_str()) );
				uvIndices    .push_back( atoi(content[5].c_str()) );
				uvIndices    .push_back( atoi(content[8].c_str()) );

				normalIndices.push_back( atoi(content[3].c_str()) );
				normalIndices.push_back( atoi(content[6].c_str()) );
				normalIndices.push_back( atoi(content[9].c_str()) );
			}
			else
			if (content.size() == 7)
			{
				vertexIndices.push_back( atoi(content[1].c_str()) );
				vertexIndices.push_back( atoi(content[3].c_str()) );
				vertexIndices.push_back( atoi(content[5].c_str()) );

				normalIndices.push_back( atoi(content[2].c_str()) );
				normalIndices.push_back( atoi(content[4].c_str()) );
				normalIndices.push_back( atoi(content[6].c_str()) );
			}
			else
			if (content.size() == 4)
			{
				vertexIndices.push_back( atoi(content[1].c_str()) );
				vertexIndices.push_back( atoi(content[2].c_str()) );
				vertexIndices.push_back( atoi(content[3].c_str()) );

			if (!first) {
			Vector3f normal = { 1, 1, 1 };
			temp_normals.push_back(normal);
			temp_normals.push_back(normal);
			temp_normals.push_back(normal);
			first = true;
			}

				normalIndices.push_back( 1 );
				normalIndices.push_back( 1 );
				normalIndices.push_back( 1 );
			}
		}
	}

	// the amount of vertices, UVs and normals must be equal
	//if (!(temp_vertices.size() == temp_uvs.size() && temp_vertices.size() == temp_normals.size()))
	//	throw 1;

	count = vertexIndices.size();
	vertices = new Vector3f[count];
	colors   = new Vector3f[count];
	normals  = new Vector3f[count];

	// for each Vector3f of each triangle
	for (uint32_t i = 0; i < vertexIndices.size(); i++)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		//unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		Vector3f vertex = temp_vertices[ vertexIndex-1 ];
		//glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		Vector3f normal = temp_normals[ normalIndex-1 ];

		vertices[i] = vertex;
		//colors[i].fromGLM(uv);
		colors[i].x = 1.0f;
		colors[i].y = 1.0f;
		colors[i].z = 1.0f;
		normals[i] = normal;
	}
	//std::cout << "Done" << vertexIndices.size() << std::endl;
}
#endif

/*

void Cube::draw() const
{
	//if ((flags & FLAG_UPDATED) == 0) update();

	// attribute buffer 0: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexHandle);
	glVertexAttribPointer(
		0,                  // attribute index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);

	// attribute buffer 1: colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorHandle);
	glVertexAttribPointer(
		1,                  // attribute index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);

	// attribute buffer 2: normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalHandle);
	glVertexAttribPointer(
		2,                                // attribute index
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		NULL                              // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, FLOAT_COUNT / 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

*/


std::ostream &operator << (
	std::ostream &out,
	const Vector3f &Vector3f )
{
	out << "[ x = " << Vector3f.x
		<< "; y = " << Vector3f.y
		<< "; y = " << Vector3f.y << "]";
}
