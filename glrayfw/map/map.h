
#pragma once

#include <fstream>
#include <vector>
#include <cml/cml.h>

#include "../core/matrix2d.h"
#include "../core/dynamicarray.h"

class Map
{

public:

	typedef int BlockType;
	static const int BLOCK_FREE = 0;
	static const int BLOCK_SOLID = 1;


private:

	struct BlockData
	{
		void Set( int x, int y, int block_type )
		{
			this->x = x;
			this->y = y;
			this->block_type = block_type;
		}

		int x, y;
		int block_type;
	};

	Matrix2D<int> map_data;
	bool modelsBufferGenerated = false;
	DynamicArray<cml::matrix44f_c> models_list[3];


public:

	Map& operator=( const Map& other )
	{
		this->map_data = other.map_data;
		return *this;
	}

	Map( const Map& other ) : map_data(other.map_data) {}
	Map(){}
	Map(int w, int h)
	{ map_data.Alloc(h,w); }

	void LoadFromFile( const char* path )
	{
		int width, height, block_type;

		std::ifstream file( path );
		file >> width >> height;

		map_data.Alloc( height, width );

		for( int i = 0; i < height; i++ )
		{
			for( int j = 0; j < width; j++ )
			{
				// NO ERROR CHECK!
				file >> block_type;
				map_data.Set( i, j, block_type );
			}
		}
	}

	void SaveToFile( const char* path )
	{
		FILE* f = fopen(path, "w+");
		fprintf(f, "%d %d ", Width(), Height());
		for( int x = 0; x < Width(); x++ )
		{
			for( int y = 0; y < Height(); y++ )
			{
				fprintf(f, "%d", Get( x, y ));
			}
			fprintf(f, "\n");
		}
		fclose(f);
	}

	bool IsFree( int x, int y )
	{
		return map_data.Get( y, x ) == BLOCK_FREE;
	}

	int Get( int x, int y )
	{
		return map_data.Get( y, x );
	}

	void Set( int x, int y, int block )
	{
		map_data.Set( y, x, block );
	}

	int Width()
	{
		return map_data.Cols();
	}

	int Height()
	{
		return map_data.Rows();
	}

	void Debug()
	{
		map_data.Debug();
	}

	int GetScroll( int x, int y )
	{
		if( x < 0 ) x = Width() + x;
		else if( x >= Width() ) x = Width() - x;
		if( y < 0 ) y = Height() + y;
		else if( y >= Height() ) y = Height() - y;
		return Get(x,y);
	}

	void GenerateModels( int block_type = 1 )
	{
		models_list[block_type-1].Clear();
		for( int i = 0; i < map_data.Rows(); i++ )
		{
			for( int j = 0; j < map_data.Cols(); j++ )
			{
				if( !IsFree( j, i ) && Get( j, i ) == block_type )
				{
					models_list[block_type-1].Add( cml::identity<4>() );
					cml::matrix_translation( models_list[block_type-1].Back(), cml::vector3f( j*2, 0, i*2 ) );
				}
			}
		}
	}

	cml::matrix44f_c* GetModelsBuffer( int num )
	{
		if( !modelsBufferGenerated )
		{
			GenerateModels(1);
			GenerateModels(2);
			GenerateModels(3);
			modelsBufferGenerated = true;
		}
		return models_list[num-1].GetRawData();
	}

	int GetModelsNum(int num)
	{
		return models_list[num-1].Size();
	}

};
