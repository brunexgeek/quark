#include <engine/opengl/Mesh.hh>
#include <engine/Exception.hh>
#include <GL/glew.h>
#include <engine/Vector3.hh>
#include <iostream>
#include <cstdlib>
#include <cstring>


namespace quark {
namespace opengl {


Mesh::Mesh(
	const MesherObject &object ) : quark::Mesh(object)
{
	glGenBuffers(1, &vertexHandle_);
	glGenBuffers(1, &normalHandle_);
	glGenBuffers(1, &uvHandle_);
	glGenBuffers(1, &faceHandle_);

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexHandle_);
	glBufferData(GL_ARRAY_BUFFER, Vector3f::SIZE * object.vertices->size(), object.vertices->data(),
		GL_STATIC_DRAW);

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, normalHandle_);
	glBufferData(GL_ARRAY_BUFFER, Vector3f::SIZE * object.normals->size(), object.normals->data(),
		GL_STATIC_DRAW);

	// UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvHandle_);
	glBufferData(GL_ARRAY_BUFFER, Vector2f::SIZE * object.uvs->size(), object.uvs->data(),
		GL_STATIC_DRAW);

	// faces
	glBindBuffer(GL_ARRAY_BUFFER, faceHandle_);
	glBufferData(GL_ARRAY_BUFFER, Vector3u::SIZE * object.faces->size(), object.faces->data(),
		GL_STATIC_DRAW);

	faceCount_ = object.faces->size();
}


Mesh::Mesh( Mesh &&obj ) : quark::Mesh(std::move(obj))
{
	vertexHandle_ = obj.vertexHandle_;
	normalHandle_ = obj.normalHandle_;
	uvHandle_     = obj.uvHandle_;
	faceHandle_   = obj.faceHandle_;
	faceCount_    = obj.faceCount_;
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &faceHandle_);
	glDeleteBuffers(1, &uvHandle_);
	glDeleteBuffers(1, &normalHandle_);
	glDeleteBuffers(1, &vertexHandle_);
}


}}