#include <engine/Camera.hh>
#include <algorithm>
#include <iostream>


Camera::Camera(
	const Vector3f &position,
	const Vector3f &target,
	float fov,
	float aspect ) : position_(position), target_(target), fov_(fov),
		aspect_(aspect), angles_(0)
{
	//target_ = glm::normalize(target_) + position_;
	// projection matrix with display range of 0.1 unit <-> 100 units
	projection_ = glm::perspective(glm::radians(fov), aspect, 0.01f, 100.0f);
	update();
}


Camera::~Camera()
{
	// nothing to do
}


const Vector3f &Camera::getPosition() const
{
	return position_;
}


const Vector3f &Camera::getTarget() const
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


void Camera::translate(
	Vector3f inc )
{
	position_ += inc;
	target_ += inc;
	update();
}


void Camera::rotatePoint(
	Vector3f &object,
	const Vector3f &angles )
{
	if (angles.x == 0 && angles.y == 0 && angles.z == 0) return;

	float angleX = glm::radians(angles.x);
	float angleY = glm::radians(angles.y);
	float angleZ = glm::radians(angles.z);

	glm::mat4 rotationMat;
	if (angleX != 0) rotationMat = glm::rotate(rotationMat, angleX, glm::vec3(1, 0, 0));
	if (angleY != 0) rotationMat = glm::rotate(rotationMat, angleY, glm::vec3(0, 1, 0));
	if (angleZ != 0) rotationMat = glm::rotate(rotationMat, angleZ, glm::vec3(0, 0, 1));
	//std::cout << "From: " << object.x << ',' << object.y << ',' << object.z << std::endl;
	glm::vec3 temp = glm::vec3(rotationMat * glm::vec4(object.x, object.y, object.z, 1.0));
	object.x = temp.x;
	object.y = temp.y;
	object.z = temp.z;
	//std::cout << "  To: " << object.x << ',' << object.y << ',' << object.z << std::endl << std::endl;
}


void Camera::rotateTarget(
	const Vector3f &angles )
{
	target_ -= position_;
	rotatePoint(target_, angles);
	target_ += position_;
	update();
}


void Camera::rotateCamera(
	const Vector3f &angles )
{
	position_ -= target_;
	rotatePoint(position_, angles);
	position_ += target_;
	update();
}