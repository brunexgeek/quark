#ifndef ENGINE_MESH_HH
#define ENGINE_MESH_HH


#include <stdint.h>
#include <iostream>
#include <engine/Mesher.hh>
#include <engine/Vector3.hh>
#include <engine/Vector2.hh>
#include <vector>


class Mesh
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
		uint32_t getFaceCount() const    { return faceCount_; }

	protected:
		uint32_t vertexHandle_;
		uint32_t normalHandle_;
		uint32_t uvHandle_;
		uint32_t faceHandle_;
		uint32_t faceCount_;

		Vector3f *computeNormal(
			const Vector3f *vetices,
			uint32_t count ) const;
};


#endif // ENGINE_MESH_HH
