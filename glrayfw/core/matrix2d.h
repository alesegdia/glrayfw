
#pragma once

#include <stdlib.h>
#include <iostream>
#include "glrayfw/core/core.h"

namespace matrix {

template <typename T>
class Matrix2D
{


private:

	int rows, cols;
	T* data;


public:

    int getScroll(int x, int y)
    {
        size_t W = Cols();
        size_t H = Rows();
        if( x < 0 ) x = W + x;
        else if( x >= W ) x = W - x;
        if( y < 0 ) y = H + y;
        else if( y >= H ) y = H - y;
        return Get(x,y);
    }


	const void* Raw()
	{
		return ((void*)data);
	}


	Matrix2D() : data( NULL )
	{

	}

	Matrix2D( int rows_, int cols_ ) : data( NULL )
	{
		rows = rows_;
		cols = cols_;
		Alloc( rows, cols );
	}

	Matrix2D( int size ) : data( NULL )
	{
		rows = size;
		cols = size;
		Alloc( rows, cols );
	}

	Matrix2D( const Matrix2D& other ) : data( NULL )
	{
		Alloc( other.rows, other.cols );

		for( int i = 0; i < this->rows; i++ )
		{
			for( int j = 0; j < this->cols; j++ )
			{
				Set( i, j, other.Get( i, j ) );
			}
		}
	}

	~Matrix2D()
	{
		if( data != NULL )
			delete [] data;
	}

	void Alloc( int rows, int cols )
	{
		if( data != NULL ) delete [] data;
		this->rows = rows;
		this->cols = cols;
		data = new T[rows * cols];
		for( int r = 0; r < rows; r++ )
			for( int c = 0; c < cols; c++ )
				Set( r, c, 0 );
	}

	/*
	T operator()(int i, int j)
	{
		return data.Get(i,j);
	}
	*/

	void Set( int row, int col, T val )
	{
		data[row * this->cols + col] = val;
	}

	void Set( T val )
	{
		for( int i = 0; i < this->rows; i++ )
		{
			for( int j = 0; j < this->cols; j++ )
			{
				Set( i, j, val );
			}
		}
	}

	T Get( int row, int col ) const
	{
		return data[row * this->cols + col];
	}

	/*
	Matrix2D& operator=( const Matrix2D& other )
	{
		for( int i = 0; i < cols; i++ )
		for( int j = 0; j < rows; j++ )
		{
			Set(i,j,other.Get(i,j));
		}
		return *this;
	}
	*/

	void Debug()
	{
		for( int i = 0; i < rows; i++ )
		{
			for( int j = 0; j < cols; j++ )
			{
				std::cout << Get( i, j ) << "";
			}
			printf("\n");
		}
	}

    int Rows() const { return rows; }
    int Cols() const { return cols; }

	Matrix2D& operator=( const Matrix2D& other )
	{
		Alloc( other.rows, other.cols );
		for( int i = 0; i < rows; i++ )
		for( int j = 0; j < cols; j++ )
		{
			Set( i, j, other.Get(i, j) );
		}

		return *this;
	}


};

}

typedef matrix::Matrix2D<int> Matrix2D;
