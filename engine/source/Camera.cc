#include <engine/Camera.hh>
#include <algorithm>
#include <iostream>


Camera::Camera(
	const glm::vec3 &position,
	const glm::vec3 &target,
	float fov,
	float aspect ) : position_(position), target_(target), fov_(fov),
		aspect_(aspect), angles_(0)
{
	target_ = glm::normalize(target_) + position_;
	// projection matrix with display range of 0.1 unit <-> 100 units
	projection_ = glm::perspective(glm::radians(fov), aspect, 0.01f, 100.0f);
	update();
}


Camera::~Camera()
{
	// nothing to do
}


const glm::vec3 &Camera::getPosition() const
{
	return position_;
}


const glm::vec3 &Camera::getTarget() const
{
	return target_;
}


const glm::mat4 &Camera::getMatrix() const
{
	return matrix_;
}


const glm::mat4 &Camera::getGlobalMatrix() const
{
	return globalMatrix_;
}


void Camera::update()
{
	matrix_ = glm::lookAt(
		glm::vec3(position_.x, position_.y, position_.z),
		glm::vec3(target_.x, target_.y, target_.z),
		glm::vec3(0,1,0) ); // head is up (set to 0,-1,0 to look upside-down)
	globalMatrix_ = projection_ * matrix_;
}


void Camera::move(
	glm::vec3 inc )
{
	position_ += inc;
	target_ += inc;
	update();
}


void Camera::rotatePoint(
	glm::vec3 &object,
	const glm::vec3 &angles )
{
	if (angles.x == 0 && angles.y == 0 && angles.z == 0) return;

	auto applyAngle = [](glm::vec3 &object, float angle, const glm::vec3 &axis)
	{
		glm::mat4 rotationMat;
		rotationMat = glm::rotate(rotationMat, angle, axis);
		std::cout << "Angle: " << angle << "    Axis:" << axis.x << ',' << axis.y << ',' << axis.z << std::endl;
		std::cout << "From: " << object.x << ',' << object.y << ',' << object.z << std::endl;
		object = glm::vec3(rotationMat * glm::vec4(object, 1.0));
		std::cout << "  To: " << object.x << ',' << object.y << ',' << object.z << std::endl << std::endl;
	};

	if (angles.x != 0) applyAngle(object, angles.x, glm::vec3(1, 0, 0));
	if (angles.y != 0) applyAngle(object, angles.y, glm::vec3(0, 1, 0));
	if (angles.z != 0) applyAngle(object, angles.z, glm::vec3(0, 0, 1));
}


void Camera::rotateTarget(
	const glm::vec3 &angles )
{
	target_ -= position_;
	rotatePoint(target_, angles);
	target_ += position_;
	update();
}


void Camera::rotateCamera(
	const glm::vec3 &angles )
{
	rotatePoint(position_, angles);
	update();
}