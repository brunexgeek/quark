#ifndef QUARK_OPENGL_MESH_HH
#define QUARK_OPENGL_MESH_HH


#include <engine/Mesh.hh>
#include <engine/Mesher.hh>


namespace quark {
namespace opengl {


class Mesh : public quark::Mesh
{
	public:
		Mesh( const MesherObject &object );

		Mesh( const Mesh &obj ) = delete;

		Mesh( Mesh &&obj );

		~Mesh();

		uint32_t getFaceHandle() const   { return faceHandle_; }
		uint32_t getVertexHandle() const { return vertexHandle_; }
		uint32_t getNormalHandle() const { return normalHandle_; }
		uint32_t getUvHandle() const     { return uvHandle_; }

	protected:
		uint32_t vertexHandle_;
		uint32_t normalHandle_;
		uint32_t uvHandle_;
		uint32_t faceHandle_;
};


}}

#endif // QUARK_OPENGL_MESH_HH
