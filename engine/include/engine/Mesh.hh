#ifndef QUARK_MESH_HH
#define QUARK_MESH_HH


#include <stdint.h>
#include <iostream>
#include <engine/Mesher.hh>
#include <engine/Vector.hh>
#include <vector>


namespace quark {


class Mesh
{
	public:
		Mesh( const MesherObject &object );

		Mesh( const Mesh &obj ) = delete;

		Mesh( Mesh &&obj );

		virtual ~Mesh() = 0;

		uint32_t getFaceCount() const    { return faceCount_; }

	protected:
		uint32_t faceCount_;
};

}

#endif // QUARK_MESH_HH
