#ifndef ENGINE_LIGHT_HH
#define ENGINE_LIGHT_HH


#include <glm/gtc/matrix_transform.hpp>


class Light
{
	public:
		Light(
			const Vector3f &position ) : position(position)
		{

		}

		virtual ~Light()
		{
		}

		const Vector3f &getPosition() const
		{
			return position;
		}

		void setPosition( const Vector3f &pos )
		{
			position = pos;
			std::cout << "Light " << this << " moved to [x: " << position.x << ", y: " << position.y << ", z: " << position.z << "]\n";
		}

		void rotateZ(
			float degree )
		{
			glm::mat4 rotationMat;
			rotationMat = glm::rotate(rotationMat, degree, glm::vec3(0.0, 1.0, 0.0));
			glm::vec3 temp = glm::vec3(rotationMat * glm::vec4(position.x, position.y, position.z, 1.0));
			position.x = temp.x;
			position.y = temp.y;
			position.z = temp.z;
		}

	protected:
		Vector3f position;
};


#endif // ENGINE_LIGHT_HH
