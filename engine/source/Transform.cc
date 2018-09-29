#include <engine/Transform.hh>
#include <iostream>


#define IS_VALID_ANGLE(x)  (!std::isnan(x) && (x) > 0 && (x) < 360)
#define IS_VALID_SCALE(x)  (!std::isnan(x) && (x) != 1 && (x) > 0 && !std::isinf(x))


Transform::Transform()
{
    matrix_ = Matrix4f::identity();
    scales_ = { 1.0F, 1.0F, 1.0F };
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

    // scale
    if (scales_.x != 1 || scales_.y != 1 || scales_.z != 1)
    {
        Matrix4f rot({
            scales_.x,         0,         0, 0,
                    0, scales_.y,         0, 0,
                    0,         0, scales_.z, 0,
                    0,         0,         0, 1 });
        matrix_ *= rot;
    }

    // translation
    matrix_[{0,3}] = position_.x;
    matrix_[{1,3}] = position_.y;
    matrix_[{2,3}] = position_.z;

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

void Transform::scale( float x, float y, float z )
{
    if (IS_VALID_SCALE(x)) scales_.x = x;
    if (IS_VALID_SCALE(y)) scales_.y = y;
    if (IS_VALID_SCALE(z)) scales_.z = z;
    changed_ = true;
}

void Transform::scale( const Vector3f &scales )
{
    scale(scales.x, scales.y, scales.z);
}