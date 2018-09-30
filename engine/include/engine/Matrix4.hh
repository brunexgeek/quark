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
    T *row1;
    const T *row2;

    Row4( T *row ) : row1(row), row2(nullptr)
    {
    }

    Row4( const T *row ) : row1(nullptr), row2(row)
    {
    }

    inline T& operator[]( const size_t index )
    {
        return row1[index % 4];
    }

    inline T operator[]( const size_t index ) const
    {
        if (row1)
            return row1[index % 4];
        else
            return row2[index % 4];
    }
};


struct MatrixIndex
{
    size_t y;
    size_t x;
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

    Matrix4<T> operator*( const Matrix4 &obj ) const
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

    Row4<T> operator[]( const size_t y )
    {
        return Row4<T>(data + y * 4);
    }

    const Row4<T> operator[]( const size_t y ) const
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

    Matrix4<T> transpose() const
    {
        Matrix4<T> result;

        result[{0,0}] = (*this)[{0,0}];
        result[{1,0}] = (*this)[{0,1}];
        result[{2,0}] = (*this)[{0,2}];
        result[{3,0}] = (*this)[{0,3}];

        result[{0,1}] = (*this)[{1,0}];
        result[{1,1}] = (*this)[{1,1}];
        result[{2,1}] = (*this)[{1,2}];
        result[{3,1}] = (*this)[{1,3}];

        result[{0,2}] = (*this)[{2,0}];
        result[{1,2}] = (*this)[{2,1}];
        result[{2,2}] = (*this)[{2,2}];
        result[{3,2}] = (*this)[{2,3}];

        result[{0,3}] = (*this)[{3,0}];
        result[{1,3}] = (*this)[{3,1}];
        result[{2,3}] = (*this)[{3,2}];
        result[{3,3}] = (*this)[{3,3}];

        return result;
    }

};


typedef Matrix4<float> Matrix4f;


#if 1

#include <iostream>

template <typename T>
static std::ostream &operator << ( std::ostream &out, const Matrix4<T> &value )
{
    for (size_t y = 0; y < 4; ++y)
        out << value[y][0] << ", " << value[y][1] << ", " << value[y][2] << ", " << value[y][3] << std::endl;;
    return out;
}

#endif


#endif // ENGINE_MATRIX4_HH