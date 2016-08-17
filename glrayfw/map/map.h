
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
		void Set( int x, int y, int block_type );

		int x, y;
		int block_type;
	};

	Matrix2D<int> map_data;
	bool modelsBufferGenerated = false;
	DynamicArray<cml::matrix44f_c> models_list[3];

public:

	Map& operator=( const Map& other );
	Map( const Map& other );
	Map();
	Map(int w, int h);

	void LoadFromFile( const char* path );
	void SaveToFile( const char* path );

	bool IsFree( int x, int y );

	int Get( int x, int y );
	void Set( int x, int y, int block );

	int Width();
	int Height();

	void Debug();

	int GetScroll( int x, int y );

	void GenerateModels( int block_type = 1 );
	cml::matrix44f_c* GetModelsBuffer( int num );
	int GetModelsNum(int num);

};
