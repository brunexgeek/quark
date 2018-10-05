#include <engine/Mesh.hh>
#include <engine/Exception.hh>
#include <GL/glew.h>
#include <engine/Vector.hh>
#include <iostream>
#include <cstdlib>
#include <cstring>


namespace quark {


Mesh::Mesh(
	const MesherObject &object ) : faceCount_(0)
{
	faceCount_ = (uint32_t) object.faces->size();
}


Mesh::Mesh( Mesh &&obj )
{
	faceCount_ = obj.faceCount_;
}


Mesh::~Mesh()
{
}


}