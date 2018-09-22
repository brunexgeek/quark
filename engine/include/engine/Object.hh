#ifndef ENGINE_OBJECT_HH_
#define ENGINE_OBJECT_HH_


#include <stdint.h>
#include <engine/Mesh.hh>


class Renderer;
class Material;


class Object
{
	public:
		Object(
			const Mesh &mesh );

		Object(
			const Object &object );

		virtual ~Object();

		virtual void draw(
			Renderer &renderer );

		virtual void update();

		void setPosition(
			const Vertex &value );

		const Vertex &getPosition() const;

		const Vertex &getDimension() const;

	protected:
		Vertex position;
		Vertex dimension;
		uint64_t lastUpdate;
		const Mesh &mesh;
};

#endif /* ENGINE_OBJECT_HH_ */
