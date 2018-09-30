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
	//front_ = glm::normalize(front_) + position_;
	// projection matrix with display range of 0.1 unit <-> 1000 units
	//projection_ = glm::perspective(glm::radians(fov), aspect, 0.01f, 1000.0f);
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

	projection_[{0,0}] = 1.0F / (aspect * tanHalfFovy);
	projection_[{1,1}] = 1.0F / (tanHalfFovy);
	projection_[{2,2}] = - (zFar + zNear) / (zFar - zNear);
	projection_[{2,3}] = - 1.0F;
	projection_[{3,2}] = - (2.0F * zFar * zNear) / (zFar - zNear);
}


Matrix4f Camera::lookAt(
	const Vector3f &position,
	const Vector3f &target,
	const Vector3f &up )
{
	Vector3f f(Vector3f::normalize(target - position));
	Vector3f s(Vector3f::normalize(Vector3f::cross(f, up)));
	Vector3f u(Vector3f::cross(s, f));

	Matrix4f result = Matrix4f::identity();
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
#if 0
	auto ppp = glm::lookAt(
		glm::vec3(position_.x, position_.y, position_.z),
		glm::vec3(front_.x, front_.y, front_.z),
		glm::vec3(0,1,0) );
	auto proj = glm::perspective( (float)DEGREE_TO_RAD(50.0F), Camera::AR_16x9, 0.01F, 1000.0F);
	auto glob = proj * ppp;
#endif
	matrix_ = lookAt(position_, front_, Vector3f(0.0F, 1.0F, 0.0F));
	//std::cout << &projection_ << std::endl;
	//std::cout << &matrix_ << std::endl;

	// FIXME: matrix multiplication operator is reversing its operands
	//globalMatrix_ = projection_ * matrix_;
	globalMatrix_ = matrix_ * projection_;
#if 0
	std::cout << "Projection\n";
	for (size_t y = 0; y < 4; ++y)
        std::cout << proj[y][0] << ", " << proj[y][1] << ", " << proj[y][2] << ", " << proj[y][3] << std::endl;
	std::cout << projection_ << std::endl;

	std::cout << "Matrix\n";
	for (size_t y = 0; y < 4; ++y)
        std::cout << ppp[y][0] << ", " << ppp[y][1] << ", " << ppp[y][2] << ", " << ppp[y][3] << std::endl;
	std::cout << matrix_ << std::endl;

	std::cout << "Global\n";
	for (size_t y = 0; y < 4; ++y)
        std::cout << glob[y][0] << ", " << glob[y][1] << ", " << glob[y][2] << ", " << glob[y][3] << std::endl;
	std::cout << globalMatrix_ << std::endl;

#endif
}


void Camera::translate(
	Vector3f inc )
{
	position_ += inc;
	//front_ += inc;
	update();
}


void Camera::rotatePoint(
	Vector3f &object,
	const Vector3f &angles )
{
	if (angles.x == 0 && angles.y == 0 && angles.z == 0) return;

	float angleX = DEGREE_TO_RAD(angles.x);
	float angleY = DEGREE_TO_RAD(angles.y);
	float angleZ = DEGREE_TO_RAD(angles.z);

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
	front_ -= position_;
	rotatePoint(front_, angles);
	front_ += position_;
	update();
}


void Camera::rotateCamera(
	const Vector3f &angles )
{
	position_ -= front_;
	rotatePoint(position_, angles);
	position_ += front_;
	update();
}

Vector3f Camera::leftSide() const
{
	return Vector3f::cross(up_, front_).normalize();
}

Vector3f Camera::rightSide() const
{
	return Vector3f::cross(front_, up_).normalize();
}

void Camera::pan( float angle )
{
	Vector3f axis = Vector3f::cross(up_, front_).normalize();
	front_.rotate(angle, up_);//.normalize();
	std::cout << "Front " << front_ << std::endl;
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
/*
void Camera::rotate( float angle, Vector3f axis )
{
	float sinHalfAngle = std::sin(DEGREE_TO_RAD(angle) / 2.0F);
	float cosHalfAngle = std::cos(DEGREE_TO_RAD(angle) / 2.0F);

	Quaternion rotation(
		axis.x * sinHalfAngle,
		axis.y * sinHalfAngle,
		axis.z * sinHalfAngle,
		cosHalfAngle );
	Quaternion conj = rotation.conjugate();
	Quaternion w = rotation * this * conj;


}*/
