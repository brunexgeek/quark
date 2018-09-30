#ifndef ENGINE_MATRIX4_HH
#define ENGINE_MATRIX4_HH


#include <cmath>
#include <initializer_list>
#include <cstdlib>
#include <cstring>
#include <algorithm>


template <typename T, int DIM>
struct Row
{
    T *row1;
    const T *row2;

    Row( T *row ) : row1(row), row2(nullptr)
    {
    }

    Row( const T *row ) : row1(nullptr), row2(row)
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


template <typename T, int DIM>
struct Matrix
{
    T data[DIM];

    Matrix()
    {
        // IEEE-754 "zero" have all bits off
        memset(data, 0, sizeof(data));
    }

    Matrix( const Matrix &obj )
    {
        memcpy(data, obj.data, sizeof(data));
    }

    explicit Matrix( const T *array )
    {
        memcpy(data, array, sizeof(data));
    }

    Matrix( const std::initializer_list<T> &values )
    {
        std::copy(values.begin(), values.end(), data);
    }

    static Matrix<T, DIM> identity()
    {
        Matrix<T,DIM> temp;
        temp.data[0] = (T) 1.0;
        temp.data[5] = (T) 1.0;
        temp.data[10] = (T) 1.0;
        temp.data[15] = (T) 1.0;
        return temp;
    }

    Matrix<T,DIM>& operator=( const Matrix &obj )
    {
        memcpy(data, obj.data, sizeof(data));
        return *this;
    }

    Matrix<T,DIM> operator*( const T value )
    {
        Matrix temp(*this);
        return temp *= value;
    }

    Matrix<T,DIM> operator*( const Matrix &obj ) const
    {
        Matrix temp;
        const T *a = obj.data;
        const T *b = this->data;

        for (unsigned j = 0; j < 4; j++)
        {
            unsigned j4 = j * 4;
            temp.data[j4 + 0] = b[j4] * a[0] + b[j4 + 1] * a[0 + 4] + b[j4 + 2] * a[0 + 8] + b[j4 + 3] * a[0 + 12];
            temp.data[j4 + 1] = b[j4] * a[1] + b[j4 + 1] * a[1 + 4] + b[j4 + 2] * a[1 + 8] + b[j4 + 3] * a[1 + 12];
            temp.data[j4 + 2] = b[j4] * a[2] + b[j4 + 1] * a[2 + 4] + b[j4 + 2] * a[2 + 8] + b[j4 + 3] * a[2 + 12];
            temp.data[j4 + 3] = b[j4] * a[3] + b[j4 + 1] * a[3 + 4] + b[j4 + 2] * a[3 + 8] + b[j4 + 3] * a[3 + 12];
        }
        return temp;
    }

    Matrix<T,DIM>& operator*=( const Matrix &obj )
    {
        return (*this = this->operator*(obj));
    }

    Matrix<T,DIM>& operator*=( const T value )
    {
        for (size_t i = 0; i < DIM; ++i) data[i] *= value;
        return *this;
    }

    Row<T,DIM> operator[]( const size_t y )
    {
        return Row<T,DIM>(data + y * 4);
    }

    const Row<T,DIM> operator[]( const size_t y ) const
    {
        return Row<T,DIM>(data + y * 4);
    }

    inline T& operator[]( const MatrixIndex index )
    {
        return *(data + index.y * 4 + index.x);
    }

    inline T operator[]( const MatrixIndex index ) const
    {
        return *(data + index.y * 4 + index.x);
    }

    Matrix<T,DIM> transpose() const
    {
        Matrix<T,DIM> result;

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


typedef Matrix<float, 16> Matrix4f;


#if 1

#include <iostream>
#include <iomanip>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

template <typename T, int DIM>
static std::ostream &operator << ( std::ostream &out, const Matrix<T,DIM> &value )
{
    for (size_t y = 0; y < 4; ++y)
        out << std::setw(11) << value[y][0] << ", "
            << std::setw(11) << value[y][1] << ", "
            << std::setw(11) << value[y][2] << ", "
            << std::setw(11) << value[y][3] << std::endl;
    return out;
}

#pragma GCC diagnostic pop

#endif


#endif // ENGINE_MATRIX4_HH