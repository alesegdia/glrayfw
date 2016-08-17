#include "map.h"

Map &Map::operator=(const Map &other)
{
	this->map_data = other.map_data;
	return *this;
}

Map::Map(const Map &other)
	: map_data(other.map_data)
{

}

Map::Map()
{

}

Map::Map(int w, int h)
{
	map_data.Alloc(h,w);
}

void Map::LoadFromFile(const char *path)
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

void Map::SaveToFile(const char *path)
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

bool Map::IsFree(int x, int y)
{
	return map_data.Get( y, x ) == BLOCK_FREE;
}

int Map::Get(int x, int y)
{
	return map_data.Get( y, x );
}

void Map::Set(int x, int y, int block)
{
	map_data.Set( y, x, block );
}

int Map::Width()
{
	return map_data.Cols();
}

int Map::Height()
{
	return map_data.Rows();
}

void Map::Debug()
{
	map_data.Debug();
}

int Map::GetScroll(int x, int y)
{
	if( x < 0 ) x = Width() + x;
	else if( x >= Width() ) x = Width() - x;
	if( y < 0 ) y = Height() + y;
	else if( y >= Height() ) y = Height() - y;
	return Get(x,y);
}

void Map::GenerateModels(int block_type)
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

cml::matrix44f_c *Map::GetModelsBuffer(int num)
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

int Map::GetModelsNum(int num)
{
	return models_list[num-1].Size();
}

void Map::BlockData::Set(int x, int y, int block_type)
{
	this->x = x;
	this->y = y;
	this->block_type = block_type;
}
