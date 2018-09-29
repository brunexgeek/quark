#include <engine/Transform.hh>
#include <iostream>


#define IS_VALID_ANGLE(x)  (!std::isnan(x) && !std::isinf(x) && (x) != 0)


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
    #define CCOS(x)  ((float) std::cos( DEGREE_TO_RAD(x) ))
    #define CSIN(x)  ((float) std::sin( DEGREE_TO_RAD(x) ))

    matrix_ = Matrix4f::identity();

    // translation
    matrix_[{0,3}] = position_.x;
    matrix_[{1,3}] = position_.y;
    matrix_[{2,3}] = position_.z;

    // rotation in X axis
    if (IS_VALID_ANGLE(angles_.x))
    {
        Matrix4f rot({
            1,               0,                0, 0,
            0, CCOS(angles_.x), -CSIN(angles_.x), 0,
            0, CSIN(angles_.x),  CCOS(angles_.x), 0,
            0,               0,                0, 1 });
        matrix_ *= rot;
    }
    // rotation in Y axis
    if (IS_VALID_ANGLE(angles_.y))
    {
        Matrix4f rot({
            CCOS(angles_.y), 0, CSIN(angles_.y), 0,
                          0, 1,               0, 0,
           -CSIN(angles_.y), 0, CCOS(angles_.y), 0,
                          0, 0,               0, 1 });
        matrix_ *= rot;
    }
    // rotation in Z axis
    if (IS_VALID_ANGLE(angles_.z))
    {
        std::cout << "Rotate Z\n";
        Matrix4f rot({
            CCOS(angles_.z), -CSIN(angles_.z), 0, 0,
            CSIN(angles_.z),  CCOS(angles_.z), 0, 0,
                          0,                0, 1, 0,
                          0,                0, 0, 1 });
        matrix_ *= rot;
    }

    changed_ = false;

    #undef CCOS
    #undef CSIN
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


void Transform::rotate( float x, float y, float z )
{
    if (IS_VALID_ANGLE(x)) angles_.x = x;
    if (IS_VALID_ANGLE(y)) angles_.y = y;
    if (IS_VALID_ANGLE(z)) angles_.z = z;
    changed_ = true;
}

void Transform::rotate( const Vector3f &angles )
{
    rotate(angles.x, angles.y, angles.z);
}

