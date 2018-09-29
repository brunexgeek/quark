#ifndef ENGINE_MATRIX4_HH
#define ENGINE_MATRIX4_HH


#include <cmath>
#include <initializer_list>
#include <cstdlib>
#include <cstring>
#include <algorithm>


template <typename T>
struct Row4
{
    T *row;

    Row4( T *row ) : row(row)
    {
    }

    inline T& operator[]( const size_t index )
    {
        index = index % 4;
        return row[index];
    }

    inline T operator[]( const size_t index ) const
    {
        index = index % 4;
        return row[index];
    }
};


struct MatrixIndex
{
    size_t x;
    size_t y;
};


template <typename T>
struct Matrix4
{
    static const size_t ELEMENTS = 16;
    T data[ELEMENTS];

    Matrix4()
    {
        // IEEE-754 "zero" have all bits off
        memset(data, 0, sizeof(data));
    }

    Matrix4( const Matrix4 &obj )
    {
        memcpy(data, obj.data, sizeof(data));
    }

    Matrix4( const T *array )
    {
        memcpy(data, array, sizeof(data));
    }

    Matrix4( const std::initializer_list<T> &values )
    {
        std::copy(values.begin(), values.end(), data);
    }

    static Matrix4<T> identity()
    {
        Matrix4<T> temp;
        temp.data[0] = (T) 1.0;
        temp.data[5] = (T) 1.0;
        temp.data[10] = (T) 1.0;
        temp.data[15] = (T) 1.0;
        return temp;
    }

    Matrix4<T>& operator=( const Matrix4 &obj )
    {
        memcpy(data, obj.data, sizeof(data));
        return *this;
    }

    Matrix4<T> operator*( const T value )
    {
        Matrix4 temp(*this);
        return temp *= value;
    }

    Matrix4<T> operator*( const Matrix4 &obj )
    {
        Matrix4 temp;

        for (unsigned j = 0; j < 4; j++)
        {
            unsigned j4 = j * 4;
            temp.data[j4 + 0] = data[j4] * obj.data[0] + data[j4 + 1] * obj.data[0 + 4] + data[j4 + 2] * obj.data[0 + 8] + data[j4 + 3] * obj.data[0 + 12];
            temp.data[j4 + 1] = data[j4] * obj.data[1] + data[j4 + 1] * obj.data[1 + 4] + data[j4 + 2] * obj.data[1 + 8] + data[j4 + 3] * obj.data[1 + 12];
            temp.data[j4 + 2] = data[j4] * obj.data[2] + data[j4 + 1] * obj.data[2 + 4] + data[j4 + 2] * obj.data[2 + 8] + data[j4 + 3] * obj.data[2 + 12];
            temp.data[j4 + 3] = data[j4] * obj.data[3] + data[j4 + 1] * obj.data[3 + 4] + data[j4 + 2] * obj.data[3 + 8] + data[j4 + 3] * obj.data[3 + 12];
        }
        return temp;
    }

    Matrix4<T>& operator*=( const Matrix4 &obj )
    {
        return (*this = this->operator*(obj));
    }

    Matrix4<T>& operator*=( const T value )
    {
        for (size_t i = 0; i < ELEMENTS; ++i) data[i] *= value;
    }

    T operator[]( const size_t y )
    {
        return Row4<T>(data + y * 4);
    }

    T operator[]( const size_t y ) const
    {
        return Row4<T>(data + y * 4);
    }

    inline T& operator[]( const MatrixIndex index )
    {
        return *(data + index.y * 4 + index.x);
    }

    inline T operator[]( const MatrixIndex index ) const
    {
        return *(data + index.y * 4 + index.x);
    }

};


typedef Matrix4<float> Matrix4f;


#endif // ENGINE_MATRIX4_HH