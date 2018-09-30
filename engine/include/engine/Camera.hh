#ifndef ENGINE_CAMERA_HH
#define ENGINE_CAMERA_HH


#include <engine/Vector3.hh>
#include <engine/Matrix4.hh>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
	public:
		static constexpr float AR_4x3  = 4.0F / 3.0F;
		static constexpr float AR_16x9  = 16.0F / 9.0F;

		Camera(
			const Vector3f &position,
			const Vector3f &target,
			float fov = 66.0F,
			float aspect = AR_4x3 );

		virtual ~Camera();

		constexpr const Vector3f &getPosition() const { return position_; }

		constexpr const Vector3f &getTarget() const { return target_; }

		constexpr const Matrix4f &getMatrix() const { return matrix_; }

		constexpr const Matrix4f &getGlobalMatrix() const { return globalMatrix_; }

		void rotateTarget(
			const Vector3f &angles );

		void rotateCamera(
			const Vector3f &angles );

		void translate(
			Vector3f inc );

	protected:
		Vector3f position_;
		Vector3f target_;
		Vector3f angles_;
		Matrix4f matrix_;
		Matrix4f projection_;
		Matrix4f globalMatrix_;
		float fov_;
		float aspect_;

		void initProjection(
			float fov,
			float aspect,
			float zNear,
			float zFar );

		static Matrix4f lookAt(
			const Vector3f &position,
			const Vector3f &target,
			const Vector3f &up );

		static void rotatePoint(
			Vector3f &object,
			const Vector3f &angles );

		void update();
};


#endif // ENGINE_CAMERA_HH
