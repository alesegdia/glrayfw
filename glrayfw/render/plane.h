
#pragma once

#include "mesh.h"

class Plane : public Mesh
{

public:

	Plane() :
		Mesh( AttribConfig::XY_UV )
	{
	}

	void Prepare( Render::Context* gl, int width, int height, float planewidth=1.f, float planeheight=1.f )
	{
		Alloc( 6 * width * height );
		UpdateVertices( width, height, planewidth, planeheight );

		Mesh::Prepare( gl );
	}

	void UpdateVertices( int width, int height, int planewidth, int planeheight )
	{
		int offset = 0;
		for( int i = 0; i < height; i++ )
		{
			for( int j = 0; j < width; j++ )
			{
				float left, right, top, bot;
				left = j * planewidth;
				right = (j+1) * planewidth;
				top = i * planeheight;
				bot = (i+1) * planeheight;

				GLfloat*d=vertices;
				//meshbuilder.AddVertex( left, top );
				d[0+offset] = left; d[1+offset] = top;
				d[2+offset] = right; d[3+offset] = bot;
				d[4+offset] = left; d[5+offset] = bot;

				d[6+offset] = left; d[7+offset] = top;
				d[8+offset] = right; d[9+offset] = top;
				d[10+offset] = right; d[11+offset] = bot;

				d=uvs;
				d[0+offset] = 0.f; d[1+offset] = 1.f;
				d[2+offset] = 1.f; d[3+offset] = 0.f;
				d[4+offset] = 0.f; d[5+offset] = 0.f;

				d[6+offset] = 0.f; d[7+offset] = 1.f;
				d[8+offset] = 1.f; d[9+offset] = 1.f;
				d[10+offset] = 1.f; d[11+offset] = 0.f;

				offset += 6*2;
			}
		}
	}

};
