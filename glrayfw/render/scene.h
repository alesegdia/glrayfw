#pragma once

#include <cassert>
#include <array>

#include "../core/matrix2d.h"
#include "../render/Texture.h"
#include <vector>
#include <cml/cml.h>

#include "../core/matrix2d.h"
#include "../core/dynamicarray.h"


class Scene
{
    static constexpr size_t NUM_TILES = 10;

public:
    const Matrix2D& tilemap() const
    {
        return m_tilemap;
    }

    void tilemap(Matrix2D tilemap)
    {
        m_tilemap = tilemap;
    }

    int numTiles()
    {
        return 3;
    }

    tdogl::Texture* getTextureForTile(size_t tile_id)
    {
        assert(tile_id < NUM_TILES);
        return m_textures[tile_id];
    }

    void setTextureForTile(size_t tile_id, tdogl::Texture* tex)
    {
        assert(tex);
        assert(tile_id < NUM_TILES);
        m_textures[tile_id-1] = tex;
    }

    bool isFree(int x, int y)
    {
        return tilemap().Get( x, y ) == 0;
    }

    void generateModels(int block_type)
    {
        models_list[block_type].Clear();
        for( int i = 0; i < tilemap().Rows(); i++ )
        {
            for( int j = 0; j < tilemap().Cols(); j++ )
            {
                if( !isFree( j, i ) && tilemap().Get( j, i ) == block_type + 1 )
                {
                    models_list[block_type].Add( cml::identity<4>() );
                    cml::matrix_translation( models_list[block_type].Back(), cml::vector3f( j*2, 0, i*2 ) );
                }
            }
        }
    }

    cml::matrix44f_c *getModelsBuffer(int num)
    {
        if( !modelsBufferGenerated )
        {
            generateModels(0);
            generateModels(1);
            generateModels(2);
            modelsBufferGenerated = true;
        }
        return models_list[num].GetRawData();
    }

    int getModelsNum(int num)
    {
        return models_list[num].Size();
    }

    Matrix2D& tilemap()
    {
        return m_tilemap;
    }

    void setFloorTexture(tdogl::Texture* floor_tex)
    {
        m_floorTex = floor_tex;
    }

    void setRoofTexture(tdogl::Texture* roof_tex)
    {
        m_roofTex = roof_tex;
    }

    tdogl::Texture* floorTex()
    {
        return m_floorTex;
    }

    tdogl::Texture* roofTex()
    {
        return m_roofTex;
    }


private:


    struct BlockData
    {
        void Set(int x, int y, int block_type)
        {
            this->x = x;
            this->y = y;
            this->block_type = block_type;
        }

        int x, y;
        int block_type;
    };

    Matrix2D m_tilemap;
    std::array<tdogl::Texture*, 10> m_textures;
    DynamicArray<cml::matrix44f_c> models_list[3];
    bool modelsBufferGenerated = false;

    tdogl::Texture* m_roofTex;
    tdogl::Texture* m_floorTex;


};
