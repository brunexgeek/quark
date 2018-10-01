#ifndef ENGINE_VECTOR2_HH
#define ENGINE_VECTOR2_HH


#include <stdlib.h>
#include <cmath>


template<typename T>
struct Vector2
{
    static const size_t SIZE = sizeof(T) * 2;
    T x, y;

    Vector2()
    {
        x = y = 0;
    }

    explicit Vector2( const T &value )
    {
        x = y = value;
    }

    Vector2( const T x, const T y )
    {
        this->x = x;
        this->y = y;
    }

    Vector2( const Vector2<T> &obj )
    {
        x = obj.x;
        y = obj.y;
    }

    Vector2<T> operator+( const Vector2<T> &obj ) const
    {
        return Vector2(x + obj.x, y + obj.y);
    }

    Vector2<T> operator-( const Vector2<T> &obj ) const
    {
        return Vector2(x - obj.x, y - obj.y);
    }

    Vector2<T> operator*( const Vector2<T> &obj ) const
    {
        return Vector2(x * obj.x, y * obj.y);
    }

    Vector2<T> operator/( const Vector2<T> &obj ) const
    {
        return Vector2(x / obj.x, y / obj.y);
    }

    T length() const
    {
        return (T) sqrt(x * x + y * y);
    }

    Vector2<T>& normalize()
    {
        T len = length();
        x /= len;
        y /= len;
        return *this;
    }
};


typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

#endif // ENGINE_VECTOR2_HH