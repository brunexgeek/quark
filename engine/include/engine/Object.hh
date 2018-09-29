#ifndef ENGINE_OBJECT_HH_
#define ENGINE_OBJECT_HH_


#include <stdint.h>
#include <engine/Mesh.hh>
#include <engine/Transform.hh>


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
			const Vector3f &value );

		const Vector3f &getPosition() const;

		const Vector3f &getDimension() const;

		Transform& getTransform();

		const Transform& getTransform() const;

	protected:
		Vector3f position;
		Vector3f dimension;
		uint64_t lastUpdate;
		const Mesh &mesh;
		Transform transform;
};

#endif /* ENGINE_OBJECT_HH_ */
