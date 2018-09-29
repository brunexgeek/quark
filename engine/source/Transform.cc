#include <engine/Transform.hh>


Transform::Transform()
{
    matrix_ = Matrix4f::identity();
}


void Transform::translate( const Vector3f &value )
{
    position_ += value;
}


void Transform::moveTo( const Vector3f &value )
{
    position_ = value;
}


void Transform::update()
{
    #define DEGREE_TO_RAD(x) ( (x) * (M_PI / 180) )

    // translation
    matrix_[{0,3}] += position_.x;
    matrix_[{1,3}] += position_.y;
    matrix_[{2,3}] += position_.z;

    // rotation in X axis
    if (angles_.x != 0)
    {
        Matrix4f rot({
            1,                                  0,                                   0, 0,
            0, std::cos(DEGREE_TO_RAD(angles_.x)), -std::sin(DEGREE_TO_RAD(angles_.x)), 0,
            0, std::sin(DEGREE_TO_RAD(angles_.x)),  std::cos(DEGREE_TO_RAD(angles_.x)), 0,
            0,                                  0,                                   0, 1 });
        matrix_ *= rot;
        /*matrix_[{1,1}] = std::cos(DEGREE_TO_RAD(angles.x));
        matrix_[{1,2}] = -std::sin(__90DEGREES);
        matrix_[{2,1}] = std::sin(__90DEGREES);
        matrix_[{2,2}] = std::cos(__90DEGREES);*/

    }

    changed_ = false;

    #undef DEGREE_TO_RAD
}


const Matrix4f &Transform::getMatrix() const
{
    // TODO: better exception here
    if (changed_) throw 10;
    return matrix_;
}

const Matrix4f &Transform::getMatrix()
{
    if (changed_) update();
    return matrix_;
}

void Transform::reset()
{
    matrix_ = Matrix4f::identity();
    position_.x = position_.y = position_.z = 0.0F;
    changed_ = false;
}

void Transform::rotate( const Vector3f &angles )
{
    angles_ = angles;
    changed_ = true;
}

