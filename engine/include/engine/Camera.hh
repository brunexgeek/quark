#ifndef ENGINE_CAMERA_HH
#define ENGINE_CAMERA_HH


#include <engine/Vector.hh>
#include <engine/Matrix4.hh>
#include <engine/Transform.hh>


namespace quark {

class Camera
{
	public:
		static constexpr float AR_4x3  = 4.0F / 3.0F;
		static constexpr float AR_16x9  = 16.0F / 9.0F;

		Camera(
			const Vector3f &position,
			const Vector3f &up,
			const Vector3f &target,
			float fov = 66.0F,
			float aspect = AR_4x3 );

		virtual ~Camera();

		const Vector3f &getPosition() const { return position_; }

		const Matrix4f &getMatrix() const { return matrix_; }

		const Matrix4f &getGlobalMatrix() const { return globalMatrix_; }

		void move(
			const Vector3f &direction,
			float step );

		Vector3f leftSide() const;

		Vector3f rightSide() const;

		const Vector3f &upSide() const { return up_; };

		const Vector3f &frontSide() const { return front_; };

		Vector3f &frontSide() { return front_; };

		void tilt( float angle );

		void pan( float angle );

		void update();

	protected:
		Vector3f position_;
		Vector3f up_;
		Vector3f front_;
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


};

}

#endif // ENGINE_CAMERA_HH
