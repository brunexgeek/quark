#ifndef ENGINE_VECTOR3_HH
#define ENGINE_VECTOR3_HH


#include <cmath>
#include <stdint.h>
#include <engine/Quaternion.hh>


template<typename T>
struct Vector3
{
    T x, y, z;
    static const size_t SIZE = sizeof(T) * 3;

    Vector3()
    {
        x = y = z = 0;
    }

    explicit Vector3( const T &value )
    {
        x = y = z = value;
    }

    Vector3( T x, T y, T z )
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3( const Vector3<T> &obj )
    {
        x = obj.x;
        y = obj.y;
        z = obj.z;
    }

    Vector3<T>& operator=( const Vector3<T> &obj )
    {
        x = obj.x;
        y = obj.y;
        z = obj.z;
        return *this;
    }

    Vector3<T> operator+( const Vector3<T> &obj ) const
    {
        return Vector3(x + obj.x, y + obj.y, z + obj.z );
    }

    Vector3<T>& operator+=( const Vector3<T> &obj )
    {
        x += obj.x;
        y += obj.y;
        z += obj.z;
        return *this;
    }

    Vector3<T> operator-( const Vector3<T> &obj ) const
    {
        return Vector3(x - obj.x, y - obj.y, z - obj.z);
    }

    Vector3<T>& operator-=( const Vector3<T> &obj )
    {
        x -= obj.x;
        y -= obj.y;
        z -= obj.z;
        return *this;
    }

    Vector3<T> operator*( const Vector3<T> &obj ) const
    {
        return Vector3(x * obj.x, y * obj.y, z * obj.z);
    }

    Vector3<T> operator*( const T &value ) const
    {
        return Vector3(x * value, y * value, z * value);
    }

    Vector3<T>& operator*=( const Vector3<T> &obj )
    {
        x *= obj.x;
        y *= obj.y;
        z *= obj.z;
        return *this;
    }

    Vector3<T> operator/( const Vector3<T> &obj ) const
    {
        return Vector3(x / obj.x, y / obj.y, z / obj.z);
    }

    Vector3<T> operator/( const T &value ) const
    {
        return Vector3(x / value, y / value, z / value);
    }

    Vector3<T>& operator/=( const Vector3<T> &obj )
    {
        x /= obj.x;
        y /= obj.y;
        z /= obj.z;
        return *this;
    }

    Vector3<T>& operator-()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    Vector3<T>& normalize()
    {
        float len = length();
        x /= len;
        y /= len;
        z /= len;
        return *this;
    }

    static Vector3<T> normalize( const Vector3<T> &obj )
    {
        return obj / obj.length();
    }

    static Vector3<T> cross( const Vector3<T> &a, const Vector3<T> &b )
    {
        return Vector3<T>(
			a.y * b.z - b.y * a.z,
			a.z * b.x - b.z * a.x,
			a.x * b.y - b.x * a.y);
    }

    static T dot( const Vector3<T> &a, const Vector3<T> &b )
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    T length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3<T>& rotate( T angle, Vector3<T> axis );

};

typedef Vector3<float> Vector3f;
typedef Vector3<uint32_t> Vector3u;



template<typename T>
struct QuaternionType
{
    T x, y, z, w;

    QuaternionType() : x((T)0.0), y((T)0.0), z((T)0.0), w((T)0.0)
    {
    }

    QuaternionType( T x, T y, T z, T w ) : x(x), y(y), z(z), w(w)
    {
    }

    /*QuaternionType( Vector3<T> vec, T w ) : x(vec.x), y(vec.y), z(vec.z), w(w)
    {
    }*/

    static QuaternionType<T> normalize( const QuaternionType<T> &obj )
    {
        T len = length();
        return QuaternionType<T>(obj.x / len, obj.y / len, obj.z / len, obj.w / len);
    }

    T length() const
    {
        return srqt(x * x + y * y + z * z + w * w);
    }

    static QuaternionType<T> conjugate( const QuaternionType<T> &obj )
    {
        return QuaternionType(-obj.x, -obj.y, -obj.z, obj.w);
    }

    QuaternionType<T> operator*( const QuaternionType<T> &obj )
    {
        return QuaternionType(
            x * obj.w + w * obj.x + y * obj.z - z * obj.y,
            y * obj.w + w * obj.y + z * obj.x - x * obj.z,
            z * obj.w + w * obj.z + x * obj.y - y * obj.x,
            w * obj.w - x * obj.x - y * obj.y - z * obj.z );
    }

    QuaternionType<T> operator*( const Vector3<T> &obj )
    {
        return QuaternionType(
             w * obj.x + y * obj.z + z * obj.y,
             w * obj.y + z * obj.x + x * obj.z,
             w * obj.z + x * obj.y + y * obj.x,
            -x * obj.x - y * obj.y - z * obj.z );
    }

};


typedef QuaternionType<float> Quaternion;



template <typename T>
Vector3<T>& Vector3<T>::rotate( T angle, Vector3<T> axis )
{
    #define V3_DEGREE_TO_RAD(x) ( (float) ((x) * ( (float)M_PI / 180.0F)) )
/*
    T sinHalfAngle = (T) (std::sin(V3_DEGREE_TO_RAD(angle) / 2.0F));
    T cosHalfAngle = (T) (std::cos(V3_DEGREE_TO_RAD(angle) / 2.0F));

    Quaternion rotation(
        axis.x * sinHalfAngle,
        axis.y * sinHalfAngle,
        axis.z * sinHalfAngle,
        cosHalfAngle );
    Quaternion conj = Quaternion::conjugate(rotation);
    Quaternion w = rotation * (*this) * conj;

    x = w.x;
    x = w.x;
    x = w.x;
    x = w.x;

    return *this;*/
    angle = V3_DEGREE_TO_RAD(angle);
    #undef V3_DEGREE_TO_RAD
    const T sinAngle = sin(-angle);
    const T cosAngle = cos(-angle);

    *this = Vector3f::cross(*this, (axis * sinAngle)) +        //Rotation on local X
        (*this * cosAngle) +                     //Rotation on local Z
        axis * Vector3f::cross(*this, axis * (1 - cosAngle)); //Rotation on local Y
    return *this;
}




#if 1

#include <iostream>

static std::ostream &operator << ( std::ostream &out, const Vector3f &value )
{
	out << value.x << ' ' << value.y << ' ' << value.z;
    return out;
}

#endif



#endif // ENGINE_VECTOR3_HH