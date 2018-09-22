#ifndef ENGINE_LIGHT_HH
#define ENGINE_LIGHT_HH


#include <glm/gtc/matrix_transform.hpp>


class Light
{
	public:
		Light(
			const glm::vec3 &position ) : position(position)
		{

		}

		virtual ~Light()
		{
		}

		const glm::vec3 &getPosition() const
		{
			return position;
		}

		void rotateZ(
			float degree )
		{
			glm::mat4 rotationMat;
			rotationMat = glm::rotate(rotationMat, degree, glm::vec3(0.0, 1.0, 0.0));
			position = glm::vec3(rotationMat * glm::vec4(position, 1.0));
		}

	protected:
		glm::vec3 position;
};


#endif // ENGINE_LIGHT_HH
