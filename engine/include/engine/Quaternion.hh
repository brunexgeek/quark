#ifndef ENGINE_QUATERNION_HH
#define ENGINE_QUATERNION_HH


#include <engine/Vector3.hh>


template<typename T>
struct Quaternion
{
    T x, y, z, w;

    Quaternion() : x((T)0.0), y((T)0.0), z((T)0.0), w((T)0.0)
    {
    }

    Quaternion( T x, T y, T z, T w ) : x(x), y(y), z(z), w(w)
    {
    }

    Quaternion( Vector3<T> vec, T w ) : x(vec.x), y(vec.y), z(vec.z), w(w)
    {
    }

    Quaternion<T>& normalize()
    {
        T len = length();
        x /= len;
        y /= len;
        z /= len;
        w /= len;
        return *this;
    }

    T length() const
    {
        return srqt(x * x + y * y + z * z + w * w);
    }

    Quaternion<T> conjugate() const
    {
        return Quaternion(-x, -y, -z, w);
    }

    Quaternion<T> operator*( const Quaternion<T> &obj )
    {
        return Quaternion(
            x * obj.w + w * obj.x + y * obj.z - z * obj.y,
            y * obj.w + w * obj.y + z * obj.x - x * obj.z,
            z * obj.w + w * obj.z + x * obj.y - y * obj.x,
            w * obj.w - x * obj.x - y * obj.y - z * obj.z );
    }

    Quaternion<T> operator*( const Vector3<T> &obj )
    {
        return Quaternion(
             w * obj.x + y * obj.z + z * obj.y,
             w * obj.y + z * obj.x + x * obj.z,
             w * obj.z + x * obj.y + y * obj.x,
            -x * obj.x - y * obj.y - z * obj.z );
    }

};



#endif // #ifndef ENGINE_QUATERNION_HH