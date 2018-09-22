#include <engine/Mesh.hh>
#include <engine/Exception.hh>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>


using std::string;
using std::vector;
using std::istream;


Mesh::Mesh(
	const Vertex *vertex,
	const Vector3 *color,
	const Vector3 *normal,
	uint32_t count,
	bool isDynamic ) : count(count), isDynamic(isDynamic)
{
	initialize(vertex, color, normal, count, isDynamic);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexId);
	glDeleteBuffers(1, &colorId);
	glDeleteBuffers(1, &normalId);
}


void Mesh::initialize(
	const Vertex *vertex,
	const Vector3 *color,
	const Vector3 *normal,
	uint32_t count,
	bool isDynamic )
{
	this->count = count;
	this->isDynamic = isDynamic;

	glGenBuffers(1, &vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * count, vertex,
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	glGenBuffers(1, &colorId);
	glBindBuffer(GL_ARRAY_BUFFER, colorId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * count, color,
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	glGenBuffers(1, &normalId);
	glBindBuffer(GL_ARRAY_BUFFER, normalId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * count, normal,
		(isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}


Vertex *Mesh::computeNormal(
	const Vertex *vetices,
	uint32_t count ) const
{
	// TODO: implement this!
	return NULL;
}


uint32_t Mesh::getVertexId() const
{
	return vertexId;
}


uint32_t Mesh::getNormalId() const
{
	return normalId;
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
	const Vertex *vertex,
	uint32_t begin,
	uint32_t size )
{
	setData(vertexId, (uint8_t*) vertex, begin, size, sizeof(Vertex));
}


void Mesh::setNormal(
	const Vertex *normal,
	uint32_t begin,
	uint32_t size )
{
	setData(normalId, (uint8_t*) normal, begin, size, sizeof(Vertex));
}


void Mesh::setColor(
	const Vector3 *color,
	uint32_t begin,
	uint32_t size )
{
	setData(colorId, (uint8_t*) color, begin, size, sizeof(Vector3));
}


void Mesh::setData(
	uint32_t id,
	const uint8_t *data,
	uint32_t start,
	uint32_t count,
	uint32_t size )
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
std::cout << "Copy " << this->count << "x" << size << std::endl;
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
	Vertex *vertex;
	Vector3 *color;
	Vector3 *normal;
	uint32_t count;

	loadBin(in, vertex, color, normal, count);
	initialize(vertex, color, normal, count, isDynamic);
}


void Mesh::loadBin(
	istream &in,
	Vertex *&vertices,
	Vector3 *&colors,
	Vector3 *&normals,
	uint32_t &count )
{
	if (in.good() == false)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");

	char line[128];
	std::vector<uint32_t> vertexIndices, normalIndices;
	Vector3 *temp_vertices;
	Vector3 *temp_normals;
	vector<string> content;

	in.seekg(0x24, in.beg);
	uint32_t total;

	// reads the vertices
	in.read( (char*) &total, sizeof(uint32_t) );
	if (in.good() == false || total > 0x7FFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");
	temp_vertices = new Vector3[total]();
	in.read( (char*) temp_vertices, total * sizeof(Vector3) );

	// reads the normals
	in.read( (char*) &total, sizeof(uint32_t) );
	if (in.good() == false || total > 0x7FFFF)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from input");
	temp_normals = new Vector3[total]();
	in.read( (char*) temp_normals, total * sizeof(Vector3) );

	uint32_t groups;
	in.read( (char*) &groups, sizeof(uint32_t) );

	for (int g = 0; g < groups; ++g)
	{
		in.seekg(0x10, in.cur);
		in.read( (char*) &total, sizeof(uint32_t) );

		for (int i = 0; i < total; ++i)
		{
			uint16_t index;

			in.read( (char*) &index, sizeof(uint16_t) );
			vertexIndices.push_back(index);
			in.read( (char*) &index, sizeof(uint16_t) );
			vertexIndices.push_back(index);
			in.read( (char*) &index, sizeof(uint16_t) );
			vertexIndices.push_back(index);

			in.read( (char*) &index, sizeof(uint16_t) );
			normalIndices.push_back(index);
			in.read( (char*) &index, sizeof(uint16_t) );
			normalIndices.push_back(index);
			in.read( (char*) &index, sizeof(uint16_t) );
			normalIndices.push_back(index);
		}
	}
	// the amount of vertices, UVs and normals must be equal
	//if (!(temp_vertices.size() == temp_uvs.size() && temp_vertices.size() == temp_normals.size()))
	//	throw 1;

	count = vertexIndices.size();
	vertices = new Vertex[count];
	colors   = new Vector3[count];
	normals  = new Vector3[count];

	// for each vertex of each triangle
	for (uint32_t i = 0; i < vertexIndices.size(); i++)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		//unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		Vector3 &vertex = temp_vertices[ vertexIndex ];
		//glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		Vector3 &normal = temp_normals[ normalIndex ];

		vertices[i] = vertex;
		//colors[i].fromGLM(uv);
		colors[i].x = 0.0f;
		colors[i].y = 1.0f;
		colors[i].z = 1.0f;
		normals[i] = normal;
		/*normals[i].x = 1.0f;
		normals[i].y = 1.0f;
		normals[i].z = 1.0f;*/
	}
}


void Mesh::loadObject(
	istream &in,
	Vertex *&vertices,
	Vector3 *&colors,
	Vector3 *&normals,
	uint32_t &count )
{
	if (in.good() == false)
		throw EXCEPTION(ERR_IO_READ, 0, "Unable to read data from Wavefront input");

	char line[128];
	std::vector<uint32_t> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	//std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	vector<string> content;

	while (in.good())
	{
		line[0] = 0;
		in.getline(line, sizeof(line) - 1);

		// parses the line content
		content.clear();
		splitText(line, content);

		if (content.size() == 0) continue;

		// handles vertices
		if (content[0] == "v" && content.size() == 4)
		{
			glm::vec3 vertex;
			vertex.x = atof(content[1].c_str());
			vertex.y = atof(content[2].c_str());
			vertex.z = atof(content[3].c_str());
			temp_vertices.push_back(vertex);
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
			glm::vec3 normal;
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
		}
	}

	// the amount of vertices, UVs and normals must be equal
	//if (!(temp_vertices.size() == temp_uvs.size() && temp_vertices.size() == temp_normals.size()))
	//	throw 1;

	count = vertexIndices.size();
	vertices = new Vertex[count];
	colors   = new Vector3[count];
	normals  = new Vector3[count];

	// for each vertex of each triangle
	for (uint32_t i = 0; i < vertexIndices.size(); i++)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		//unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		//glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];

		vertices[i].fromGLM(vertex);
		//colors[i].fromGLM(uv);
		colors[i].x = 1.0f;
		colors[i].y = 1.0f;
		colors[i].z = 1.0f;
		normals[i].fromGLM(normal);
	}
}


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
	const Vertex &vertex )
{
	out << "[ x = " << vertex.x
		<< "; y = " << vertex.y
		<< "; y = " << vertex.y << "]";
}
