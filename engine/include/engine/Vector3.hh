#ifndef ENGINE_VECTOR3_HH
#define ENGINE_VECTOR3_HH


#include <cmath>
#include <stdint.h>


template<typename T>
struct Vector3
{
    T x, y, z;
    static const size_t SIZE = sizeof(T) * 3;

    Vector3()
    {
        x = y = z = 0;
    }

    Vector3( const T &value )
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

};

typedef Vector3<float> Vector3f;
typedef Vector3<uint32_t> Vector3u;

#if 1

#include <iostream>

static std::ostream &operator << ( std::ostream &out, const Vector3f &value )
{
	out << value.x << ' ' << value.y << ' ' << value.z;
    return out;
}

#endif

#endif // ENGINE_VECTOR3_HH