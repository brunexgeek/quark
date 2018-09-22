#include <engine/Camera.hh>
#include <algorithm>


Camera::Camera(
	const glm::vec3 &position,
	const glm::vec3 &target,
	float fov,
	float aspect ) : position_(position), target_(target), fov_(fov),
		aspect_(aspect)
{
	// projection matrix with display range of 0.1 unit <-> 100 units
	projection_ = glm::perspective(fov, aspect, 0.01f, 100.0f);
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
	update();
}


void Camera::rotateTarget(
	const glm::vec3 &angles,
	const glm::vec3 &axis )
{
	//glm::vec3 axis = { (float) (angles.x != 0), (float) (angles.y != 0), (float) (angles.z != 0) };
	if (axis.x < 1 && axis.y < 1 && axis.z < 1) return;

	auto applyAngle = [this](float angle, glm::vec3 axis)
	{
		if (angle == 0) return;
		glm::mat4 rotationMat = glm::rotate(rotationMat, angle, axis);
		target_ = glm::vec3(rotationMat * glm::vec4(target_, 1.0));
	};

	glm::mat4 rotationMat;
	float angle = std::max(angles.x, angles.y);

	if (angle != std::max(angle, angles.z))
	{
		// different angles
		if (axis.x > 0) applyAngle(angles.x, glm::vec3(1, 0, 0));
		if (axis.y > 0) applyAngle(angles.y, glm::vec3(0, 1, 0));
		if (axis.z > 0) applyAngle(angles.z, glm::vec3(0, 0, 1));
	}
	else
	{
		// unique angle
		applyAngle(angle, axis);
	}

	update();
}
