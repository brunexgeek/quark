#ifndef ENGINE_MESH_HH
#define ENGINE_MESH_HH


#include <stdint.h>
#include <iostream>
#include <glm/glm.hpp>


#pragma pack(push, 4)

struct Vertex
{
	float x;
	float y;
	float z;

	Vertex() : x(0), y(0), z(0)
	{
		// nothing to do
	}

	Vertex(
		float x,
		float y,
		float z ) : x(x), y(y), z(z)
	{
		// nothing to do
	}

	Vertex &fromGLM(
		const glm::vec3 &vector )
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	}
};


typedef Vertex Vector3;


std::ostream &operator << (
	std::ostream &out,
	const Vertex &vertex );


struct Vector2
{
	float u;
	float v;

	Vector2 &fromGLM(
		const glm::vec2 &vector )
	{
		u = vector.x;
		v = vector.y;
		return *this;
	}
};

#pragma pack(pop)


class Mesh
{
	public:
		Mesh(
			std::istream &in,
			bool isDynamic = false );

		Mesh(
			const Vertex *vetices,
			const Vector3 *colors,
			const Vector3 *normals,
			uint32_t count,
			bool isDynamic = false );

		~Mesh();

		uint32_t getVertexId() const;

		uint32_t getNormalId() const;

		uint32_t getColorId() const;

		uint32_t getVertexCount() const;

		void setVertex(
			const Vertex *vertex,
			uint32_t begin,
			uint32_t size );

		void setNormal(
			const Vertex *normal,
			uint32_t begin,
			uint32_t size );

		void setColor(
			const Vector3 *color,
			uint32_t begin,
			uint32_t size );

	protected:
		uint32_t vertexId;
		uint32_t normalId;
		uint32_t colorId;
		uint32_t count;
		bool isDynamic;

		void initialize(
			const Vertex *vertex,
			const Vector3 *color,
			const Vector3 *normal,
			uint32_t count,
			bool isDynamic );

		Vertex *computeNormal(
			const Vertex *vetices,
			uint32_t count ) const;

		/**
		 * @param count Amount of blocks with @c size bytes to be copied.
		 * @param start Destionation offset.
		 * @param size Size of each element.
		 */
		void setData(
			uint32_t id,
			const uint8_t *data,
			uint32_t start,
			uint32_t count,
			uint32_t size);

		void loadObject(
			std::istream &in,
			Vertex *&vertices,
			Vector3 *&colors,
			Vertex *&normals,
			uint32_t &count );

		void loadBin(
			std::istream &in,
			Vertex *&vertices,
			Vector3 *&colors,
			Vertex *&normals,
			uint32_t &count );
};


#endif // ENGINE_MESH_HH
