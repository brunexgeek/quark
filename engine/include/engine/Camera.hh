#ifndef ENGINE_CAMERA_HH
#define ENGINE_CAMERA_HH


#include <glm/gtc/matrix_transform.hpp>


class Camera
{
	public:
		static constexpr float AR_4x3  = 4.0F / 3.0F;
		static constexpr float AR_16x9  = 16.0F / 9.0F;

		Camera(
			const glm::vec3 &position,
			const glm::vec3 &target,
			float fov = 66.0F,
			float aspect = AR_4x3 );

		virtual ~Camera();

		const glm::vec3 &getPosition() const;

		const glm::vec3 &getTarget() const;

		const glm::mat4 &getMatrix() const;

		const glm::mat4 &getGlobalMatrix() const;

		void rotateTarget(
			const glm::vec3 &angles );

		void rotateCamera(
			const glm::vec3 &angles );

		void move(
			glm::vec3 inc );

	protected:
		glm::vec3 position_;
		glm::vec3 target_;
		glm::vec3 angles_;
		glm::mat4 matrix_;
		glm::mat4 projection_;
		glm::mat4 globalMatrix_;
		float fov_;
		float aspect_;

		static void rotatePoint(
			glm::vec3 &object,
			const glm::vec3 &angles );

		void update();
};


#endif // ENGINE_CAMERA_HH
