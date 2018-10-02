#include <engine/Camera.hh>
#include <algorithm>
#include <iostream>
#include <engine/Transform.hh>
#include <engine/Quaternion.hh>


Camera::Camera(
	const Vector3f &position,
	const Vector3f &up,
	const Vector3f &target,
	float fov,
	float aspect ) : position_(position), up_(up), front_(target * 1000.0F), angles_(0),
		fov_(fov), aspect_(aspect)
{
	initProjection(fov, aspect, 0.01F, 1000.0F);
	update();
}


Camera::~Camera()
{
	// nothing to do
}


void Camera::initProjection(
	float fov,
	float aspect,
	float zNear,
	float zFar )
{
	float tanHalfFovy = std::tan(DEGREE_TO_RAD(fov) / 2.0F);

	// line oriented matrix
	projection_[{0,0}] = 1.0F / (aspect * tanHalfFovy);
	projection_[{1,1}] = 1.0F / (tanHalfFovy);
	projection_[{2,2}] = - (zFar + zNear) / (zFar - zNear);
	projection_[{3,2}] = - 1.0F;
	projection_[{2,3}] = - (2.0F * zFar * zNear) / (zFar - zNear);
}


Matrix4f Camera::lookAt(
	const Vector3f &position,
	const Vector3f &target,
	const Vector3f &up )
{
	Vector3f f(Vector3f::normalize(target - position));
	Vector3f s(Vector3f::normalize(Vector3f::cross(f, up)));
	Vector3f u(Vector3f::cross(s, f));

	// line oriented matrix
	Matrix4f result(Matrix4f::identity());
	result[{0,0}] = s.x;
	result[{1,0}] = s.y;
	result[{2,0}] = s.z;
	result[{0,1}] = u.x;
	result[{1,1}] = u.y;
	result[{2,1}] = u.z;
	result[{0,2}] =-f.x;
	result[{1,2}] =-f.y;
	result[{2,2}] =-f.z;
	result[{3,0}] =-Vector3f::dot(s, position);
	result[{3,1}] =-Vector3f::dot(u, position);
	result[{3,2}] = Vector3f::dot(f, position);

	return result;
}


void Camera::update()
{
	matrix_ = lookAt(position_, front_, Vector3f(0.0F, 1.0F, 0.0F)).transpose();
	globalMatrix_ = projection_ * matrix_;
}


void Camera::move(
	const Vector3f &direction,
	float step )
{
	position_ += direction * step;
	//front_ += inc;
	update();
}

Vector3f Camera::leftSide() const
{
	return Vector3f::cross(front_, up_).normalize();
}

Vector3f Camera::rightSide() const
{
	return Vector3f::cross(up_, front_).normalize();
}

void Camera::pan( float angle )
{
	Vector3f axis = Vector3f::cross(up_, front_).normalize();
	front_.rotate(angle, up_);//.normalize();
	up_ = Vector3f::cross(front_, axis).normalize();
	update();
}

void Camera::tilt( float angle )
{
	Vector3f axis = Vector3f::cross(up_, front_).normalize();
	front_.rotate(angle, axis);//.normalize();
	up_ = Vector3f::cross(front_, axis).normalize();
	update();
}
